#include "common/kyrin_log.h"
#include "common/crypto/kyrin_base64.h"
#include "common/kyrin_macros.h"
#include "common/kyrin_lexicographically_helper.h"
#include "io/kyrin_http_client.h"
#include "upload_file_request_handler.h"
#include "protobuf/upload_file.pb.h"
#include "protobuf/get_oplog.pb.h"
#include "protobuf/operation_log.pb.h"
#include "server/chunk/kyrin_chunk_gossiper_status.h"

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;
using namespace kyrin::io;

static KyrinLog *logger = KyrinLog::get_instance();
void UploadFileRequestHandler::handle_request(KyrinMasterServer *server, evhttp_request *req)
{
    KyrinMasterStatus status;
    string reply = "Service denied...";
    if (!m_sentinel->get_status(status)) {
        server->server_send_reply_bad(req, reply);
        return ;
    }
    while (status == k_status_consensus);
    if (status == k_status_follower) {
        server->server_send_reply_bad(req, reply);
        return ;
    }

    string request_body = "";
    if (!server->server_get_postdata(req, request_body)) {
        reply = "Can't read post data...";
        server->server_send_reply_bad(req, reply);
        return ;
    }

    //// Collect data

    KyrinCluster *cluster = KyrinCluster::get_instance();
    request_body = crypto::base64_decode(request_body);
    kyrinbox::api::UploadFileRequest request_pb;
    request_pb.ParseFromString(request_body);
    logger->log("pb data account",     request_pb.account().c_str());
    logger->log("pb data file name",   request_pb.file_name().c_str());
    logger->log("pb data merkle sha1", request_pb.merkle_sha1().c_str());
    string timestamp = "";
    server->server_get_header(req, "KYRIN-TIMESTAMP", timestamp);
    logger->log("header data timestamp", timestamp.c_str());
    string signature = "";
    server->server_get_header(req, "KYRIN-SIGNATURE", signature);
    logger->log("header data signature", signature.c_str());

    //// Process data

    kyrinbox::api::UploadFileOperation operation;
    kyrinbox::api::UploadFileResponse *response = operation.mutable_data();
    pair<const char *, int> to_gossip = cluster->get_random_chunk_seed_for_gossip();

    string gossip_response;
    string gossip_request = "Read"; /* FIXME: hardcode */
    KyrinHttpClient::make_request_post(to_gossip.first,
                                       to_gossip.second,
                                       "/GetStatus",
                                       gossip_response,
                                       gossip_request);
    logger->log("gossip_response", gossip_response.c_str());
    gossip_response = crypto::base64_decode(gossip_response);
    KyrinChunkGossiperStatus chunk_status;
    chunk_status.from_string(gossip_response);
    vector<pair<uint32_t, ChunkStatusConfig> > hosts;
    if (!chunk_status.get_random_alive_3(hosts)) {
        reply = "Chunk server not ready...";
        server->server_send_reply_bad(req, reply);
        return ;
    }
    for (uint32_t i=0; i<3; i++) {
        string *new_file_hosts = response->add_file_hosts();
        char gsp_port[30]; /* FIXME: hardcode */
        sprintf(gsp_port, "%s %u", hosts[i].second.host.c_str(), hosts[i].first);
        new_file_hosts->assign(gsp_port, strlen(gsp_port));
    }
    response->set_file_size(request_pb.file_size());
    response->set_merkle_sha1(request_pb.merkle_sha1());
    string operation_data;
    if (!response->SerializeToString(&operation_data)) {
        reply = "Fail to serialize protobuf";
        server->server_send_reply_bad(req, reply);
        return ;
    }

    operation.set_key(request_pb.account()+request_pb.file_name()); /* FIXME */
    string value;
    if (m_userdata_db->exist(operation.key())) {
        reply = operation.key() + "Filename existed...";
        server->server_send_reply_bad(req, reply);
        return ;
    }

    logger->log("Two phase", "start");
    //// Two phase
    kyrinbox::api::OperationLogMessage message;
    string last_key;
    if (kyrin_likely(m_oplog_db->fetch_last_key(last_key))) {
        KyrinLexicographicallyHelper::add_one(last_key);
    } else {
        KyrinLexicographicallyHelper::get_zero(last_key);
    }
    message.set_op_id(last_key);
    message.set_state(1);

    kyrinbox::api::OperationLog *op_log = message.mutable_op_log();
    operation.SerializeToString(op_log->mutable_log_data());
    op_log->set_op_type(1);
    string confirm_response;
    string message_post;
    message.SerializeToString(&message_post);
    message_post = crypto::base64_encode((unsigned char const*)message_post.c_str(), message_post.length());
    uint32_t tp_cnt = 1;
    uint32_t master_count = cluster->get_master_count();
    for (uint32_t i=1; i<=master_count; i++) {
        if (i == m_kbid)
            continue;
        string tp_res;
        KyrinHttpClient::make_request_post(cluster->get_master_ip(i),
                                           cluster->get_master_confirm_oplog_port(i),
                                           "/ConfirmOplog",
                                           tp_res,
                                           message_post);
        if (tp_res == "ok") {
            tp_cnt++;
        }
    }
    if (tp_cnt <= (master_count>>1)) {
        reply = "Two phase rejected..";
        server->server_send_reply_bad(req, reply);
        return ;
    }
    kyrinbox::api::OperationLogMessage message_confirm;
    message_confirm.set_state(2);
    message_confirm.set_op_id(message.op_id());
    message_confirm.SerializeToString(&message_post);
    message_post = crypto::base64_encode((unsigned char const*)message_post.c_str(), message_post.length());
    for (uint32_t i=1; i<=master_count; i++) {
        if (i == m_kbid)
            continue;
        string tp_res;
        KyrinHttpClient::make_request_post(cluster->get_master_ip(i),
                                          cluster->get_master_confirm_oplog_port(i),
                                          "/ConfirmOplog",
                                          tp_res,
                                          message_post);
    }
    //// Two phase confirm success
    string op_log_str;
    if (!op_log->SerializeToString(&op_log_str)) {
        reply = "Serialize op_log failed";
        server->server_send_reply_bad(req, reply);
        return ;
    }
    if (!m_oplog_db->put(last_key, op_log_str)) {
        reply = "put in oplog failed";
        server->server_send_reply_bad(req, reply);
        return ;
    }
    if (!m_userdata_db->put(operation.key(), operation_data)) {
        reply = "put in userdata failed";
        m_oplog_db->remove(last_key);
        server->server_send_reply_bad(req, reply);
        return ;
    }

    reply = crypto::base64_encode((unsigned char const*)operation_data.c_str(), operation_data.length());
    server->server_send_reply_ok(req, reply);
    return ;
 
}

} /* server */
} /* kyrin */
