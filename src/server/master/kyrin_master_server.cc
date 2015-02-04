#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "kyrin_master_server.h"
#include "common/kyrin_macros.h"
#include "common/kyrin_log.h"
#include "common/crypto/kyrin_base64.h"
#include "common/kyrin_lexicographically_helper.h"
#include "common/kyrin_config.h"
#include "common/kyrin_constants.h"
#include "protobuf/upload_file.pb.h"
#include "protobuf/get_oplog.pb.h"
#include "protobuf/operation_log.pb.h"

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;
using namespace kyrin::io;

static KyrinLog *logger = KyrinLog::get_instance();

static void get_oplog_handler(evhttp_request *req, void *arg)
{
    KyrinMasterServer *server = (KyrinMasterServer *) arg;
    string reply = "";
    string request_body = "";
    if (!server->server_get_postdata(req, request_body)) {
        reply = "Can't read post";
        server->server_send_reply_ok(req, reply);
        return ;
    }
    request_body = crypto::base64_decode(request_body);

    if (!server->get_oplog_db()->exist(request_body)) {
        reply = "No such op id";
        server->server_send_reply_ok(req, reply);
        return ;
    }

    leveldb::Iterator* it = server->get_oplog_db()->new_iterator();
    kyrinbox::api::GetOplogResponse response;
    int cnt = 0;
    it->Seek(request_body);
    if (!it->Valid()) {
        reply = "Iterator invalid";
        server->server_send_reply_ok(req, reply);
        return ;
    }
    while (true) {
        string key = it->key().ToString();

        *(response.add_log_data()) = it->value().ToString();
        it->Next();
        if (!(it->Valid()) || cnt == constants::k_server_max_get_oplog_size) {
            response.set_last_id(key);
            break;
        }
    }
    delete it;

    response.SerializeToString(&reply);
    reply = crypto::base64_encode((unsigned char const*)reply.c_str(), reply.length());
    server->server_send_reply_ok(req, reply);
}

static void upload_file_leader_handler(evhttp_request *req, KyrinMasterServer *server)
{
    string request_body = "";
    string reply = "";
    if (!server->server_get_postdata(req, request_body)) {
        reply = "Can't read postdata";
        server->server_send_reply_ok(req, reply);
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
    response->set_file_size(1024);
    response->set_merkle_sha1("sha1sha1sha1sha1sha1sha1sha1sha1sha1sha1");
    string operation_data;
    if (!response->SerializeToString(&operation_data)) {
        reply = "Fail to serialize protobuf";
        server->server_send_reply_ok(req, reply);
        return ;
    }

    operation.set_key(request_pb.account()+request_pb.file_name());
    string value;
    if (server->get_userdata_db()->get(operation.key(), value)) {
        reply = "Filename existed";
        server->server_send_reply_ok(req, reply);
        return ;
    }

    ////  Two phase
    kyrinbox::api::OperationLogMessage message;
    string last_key;
    if (kyrin_likely(server->get_oplog_db()->fetch_last_key(last_key))) {
        KyrinLexicographicallyHelper::add_one(last_key);
    } else {
        KyrinLexicographicallyHelper::get_zero(last_key);
    }
    message.set_op_id(last_key);

    kyrinbox::api::OperationLog *op_log = message.mutable_op_log();
    string op_protobuf;
    operation.SerializeToString(op_log->mutable_log_data());
    op_log->set_op_type(1);


    /// Two phase confirm success
    string op_log_str;
    if (!op_log->SerializeToString(&op_log_str)) {
        reply = "Serialize op_log failed";
        server->server_send_reply_ok(req, reply);
        return ;
    }
    if (!server->get_oplog_db()->put(last_key, op_log_str)) {
        reply = "put in oplog failed";
        server->server_send_reply_ok(req, reply);
        return ;
    }
    if (!server->get_userdata_db()->put(operation.key(), operation_data)) {
        reply = "put in userdata failed";
        server->get_oplog_db()->remove(last_key);
        server->server_send_reply_ok(req, reply);
        return ;
    }

    server->server_send_reply_ok(req, operation_data);
    return ;   
}

static void upload_file_follower_handler(evhttp_request *req, KyrinMasterServer *server)
{
    
}

static void upload_file_handler(evhttp_request *req, void *arg)
{
    KyrinMasterServer *server = (KyrinMasterServer *) arg;
    KyrinMasterStatus status;
    if (!server->get_sentinel()->get_status(status)) {
        evhttp_send_reply_end(req);
        return ;
    }
    while (status == k_status_consensus);
    if (status == k_status_follower) {
        upload_file_follower_handler(req, server);
    } else {
        upload_file_leader_handler(req, server);
    }
}

bool KyrinMasterServer::server_initialize(KyrinMasterSentinel *sentinel)
{
    if (sentinel == NULL) {
        return false;
    }
    m_sentinel = sentinel;
    if (!server_initialize_kyrin_server_socket(upload_file_fd,
        atoi(KyrinConfig::get_instance()->get_config(constants::k_json_master_upload_file_port).c_str()),
        atoi(KyrinConfig::get_instance()->get_config(constants::k_json_master_upload_file_backlog).c_str()))) {
        return false;
    }
    if (!server_initialize_kyrin_server_socket(get_oplog_fd,
        atoi(KyrinConfig::get_instance()->get_config(constants::k_json_master_get_oplog_port).c_str()),
        atoi(KyrinConfig::get_instance()->get_config(constants::k_json_master_get_oplog_backlog).c_str()))) {
        return false;
    }

    m_userdata_db = new KyrinDatabaseWrapper(KyrinConfig::get_instance()->get_config(constants::k_json_master_userdata_database_path));
    m_oplog_db    = new KyrinDatabaseWrapper(KyrinConfig::get_instance()->get_config(constants::k_json_master_oplog_database_path));
    return true;
}

bool KyrinMasterServer::server_free()
{
    KyrinBaseServer::server_free();
    close(upload_file_fd);
    return true;
}

bool KyrinMasterServer::server_start()
{
    server_run(atoi(KyrinConfig::get_instance()->get_config(constants::k_json_master_server_threads).c_str()));
    return true;
}

bool KyrinMasterServer::server_set_processor(evhttp *server, int thread_code)
{
    if (!thread_code) {
        server_put_callback(server, "/UploadFile", upload_file_handler, this);
        server_set_evhttp_accept_socket(server, upload_file_fd);
    } else {
        server_put_callback(server, "/GetOplog", get_oplog_handler, this);
        server_set_evhttp_accept_socket(server, get_oplog_fd);
    }
    return true;
}

} /* server */
} /* kyrin */
