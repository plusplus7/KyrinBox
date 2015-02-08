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
    ((KyrinMasterServer *) arg)->get_get_oplog_request_handler()->handle_request((KyrinMasterServer *) arg, req);
}

static void upload_file_handler(evhttp_request *req, void *arg)
{
    ((KyrinMasterServer *) arg)->get_upload_file_request_handler()->handle_request((KyrinMasterServer *) arg, req);
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
    upload_file_request_handler = new UploadFileRequestHandler(m_sentinel, m_userdata_db, m_oplog_db);
    get_oplog_request_handler = new GetOplogRequestHandler(m_oplog_db);

    return true;
}

bool KyrinMasterServer::server_free()
{
    KyrinBaseServer::server_free();
    close(upload_file_fd);
    close(get_oplog_fd);
    delete m_userdata_db;
    delete m_oplog_db;
    delete upload_file_request_handler;
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
