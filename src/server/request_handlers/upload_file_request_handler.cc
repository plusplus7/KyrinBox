#include "common/kyrin_log.h"
#include "common/crypto/kyrin_base64.h"
#include "common/kyrin_macros.h"
#include "common/kyrin_lexicographically_helper.h"
#include "io/kyrin_http_client.h"
#include "upload_file_request_handler.h"
#include "protobuf/upload_file.pb.h"
#include "protobuf/get_oplog.pb.h"
#include "protobuf/operation_log.pb.h"

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
    for (int i=0; i<3; i++) {
        string *new_file_hosts = response->add_file_hosts();
        new_file_hosts->assign("127.0.0.1");
    }
    response->set_file_size(request_pb.file_size());
    response->set_merkle_sha1(request_pb.merkle_sha1());
    string operation_data;
    if (!response->SerializeToString(&operation_data)) {
        reply = "Fail to serialize protobuf";
        server->server_send_reply_bad(req, reply);
        return ;
    }

    operation.set_key(request_pb.account()+request_pb.file_name());
    string value;
    if (m_userdata_db->exist(operation.key())) {
        reply = "Filename existed...";
        server->server_send_reply_bad(req, reply);
        return ;
    }
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
    string op_protobuf;
    operation.SerializeToString(op_log->mutable_log_data());
    op_log->set_op_type(1);
    string confirm_response;
    string message_post;
    message.SerializeToString(&message_post);
    message_post = crypto::base64_encode((unsigned char const*)message_post.c_str(), message_post.length());
    uint32_t tp_cnt = 1;
    for (uint32_t i=0; i<m_masters.size(); i++) {
        if (i == m_kbid)
            continue;
        string tp_res;
        KyrinHttpClient::make_request_post(m_masters[i].ip,
                                          m_masters[i].confirm_oplog_port,
                                          "/ConfirmOplog",
                                          tp_res,
                                          message_post);
        if (tp_res == "ok") {
            tp_cnt++;
        }
    }
    if (tp_cnt <= (m_masters.size()>>1)) {
        reply = "Two phase rejected..";
        server->server_send_reply_bad(req, reply);
        return ;
    }
    kyrinbox::api::OperationLogMessage message_confirm;
    message_confirm.set_state(2);
    message_confirm.set_op_id(message.op_id());
    message_confirm.SerializeToString(&message_post);
    message_post = crypto::base64_encode((unsigned char const*)message_post.c_str(), message_post.length());
    for (uint32_t i=0; i<m_masters.size(); i++) {
        if (i == m_kbid)
            continue;
        string tp_res;
        KyrinHttpClient::make_request_post(m_masters[i].ip,
                                          m_masters[i].confirm_oplog_port,
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

    server->server_send_reply_ok(req, operation_data);
    return ;
 
}

} /* server */
} /* kyrin */
