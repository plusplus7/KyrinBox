#include "kyrin_master_sentinel_server.h"
#include "common/kyrin_log.h"
#include <unistd.h>

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

static KyrinLog *logger = KyrinLog::get_instance();

static void get_sentinel_status_handler(evhttp_request *req, void *arg)
{
    KyrinMasterSentinelServer* server = (KyrinMasterSentinelServer* ) arg;
    string message = "";
    if (server->get_sentinel()->get_status(message) == false) {
        logger->log("get_sentinel_status", "get failed...");
    }
    server->server_send_reply_ok(req, message);
}

KyrinMasterSentinel* KyrinMasterSentinelServer::get_sentinel()
{
    return m_sentinel;
}

bool KyrinMasterSentinelServer::server_initialize(KyrinMasterSentinel *sentinel, int port, int backlog)
{
    if (!server_initialize_kyrin_server_socket(server_listen_fd, port, backlog)) {
        return false;
    }
    m_sentinel = sentinel;
    return true;
}

bool KyrinMasterSentinelServer::server_set_processor(evhttp *server, int thread_code)
{
    server_put_callback(server, "/get_status", get_sentinel_status_handler, this);
    server_set_evhttp_accept_socket(server, server_listen_fd);
    return true;
}

bool KyrinMasterSentinelServer::server_start() {
    server_run(1);
    return true;
}

bool KyrinMasterSentinelServer::server_close() {
    KyrinBaseServer::server_free();
    close(server_listen_fd);
}

} /* server */
} /* kyrin */
