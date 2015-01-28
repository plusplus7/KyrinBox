#include <iostream>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "kyrin_master_server.h"
#include "common/kyrin_log.h"
#include "common/kyrin_config.h"
#include "common/kyrin_constants.h"
#include "protobuf/upload_file.pb.h"

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;
using namespace kyrin::io;

static KyrinLog *logger = KyrinLog::get_instance();

static void upload_file_leader_handler(evhttp_request *req, KyrinMasterServer *server)
{
    string request_body = "";
    string reply = "";
    if (!server->server_get_postdata(req, request_body)) {
        reply = "Can't read postdata";
        server->server_send_reply_bad(req, reply);
        return ;
    }

    //// Collect data

    kyrinbox::api::UploadFileRequest request_pb;
    request_pb.ParseFromString(request_body);
    logger->log("pb data account",     request_pb.account().c_str());
    logger->log("pb data file name",   request_pb.file_name().c_str());
    logger->log("pb data merkle sha1", request_pb.merkle_sha1().c_str());
    cout << request_pb.file_size() << endl;
    string timestamp = "";
    server->server_get_header(req, "KYRIN-TIMESTAMP", timestamp);
    logger->log("header data timestamp", timestamp.c_str());
    string signature = "";
    server->server_get_header(req, "KYRIN-SIGNATURE", signature);
    logger->log("header data signature", signature.c_str());

    //// Process data

    UploadFileOperation operation;
    kyrinbox::api::UploadFileResponse *response = operation.mutable_data();
    for (int i=0; i<3; i++) {
        string *new_file_hosts = response_pb->add_file_hosts();
        new_file_hosts->assign("127.0.0.1");
    }
    response_pb->set_file_size(1024);
    response_pb->set_merkle_sha1("sha1sha1sha1sha1sha1sha1sha1sha1sha1sha1");
    if (!response_pb.SerializeToString(&reply)) {
        reply = "Fail to serialize protobuf";
        server->server_send_reply_bad(req, reply);
        return ;
    }
    operation.set_key(request_pb.account()+request_pb.file_name());
    string value;
    if (server->get_userdata_db()->database_get(operation.key(), value)) {
        reply = "Filename existed";
        server->server_send_reply_bad(req, reply);
        return ;
    }

    ////  Two phase
    string two_phase_msg;
    operation.SerializeToString(&two_phase_msg);

    OperationLog op_log;
    op_log.set_op_type(1);
    op_log.set_log_data(two_phase_msg);
    string confirm_msg;
    op_log.SerializeToString(confirm_msg);
    KyrinHttpClient::make_request_post()


    server->server_send_reply_ok(req, reply);
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
        atoi(KyrinConfig::get_instance()->get_config(constants::k_json_master_reader_port).c_str()),
        atoi(KyrinConfig::get_instance()->get_config(constants::k_json_master_reader_backlog).c_str()))) {
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

    }
    return true;
}

} /* server */
} /* kyrin */
