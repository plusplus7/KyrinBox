#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <string>
#include <unistd.h>
#include "kyrin_master_server.h"
#include "io/kyrin_database_wrapper.h"
#include "common/kyrin_log.h"
#include <iostream>

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

static KyrinLog *logger = KyrinLog::get_instance();
static void db_get_handler(evhttp_request *req, void *arg)
{
    KyrinBaseServer *server = (KyrinBaseServer *) arg;
    string post = "";
    string reply = "";
    server->server_get_postdata(req, post);
    logger->log("db_get_handler", post.c_str());
    server->server_get_database()->database_get(post, reply);
    server->server_send_reply_ok(req, reply);
}

static void db_put_handler(evhttp_request *req, void *arg)
{
    KyrinBaseServer *server = (KyrinBaseServer *) arg;
    string post = "";
    server->server_get_postdata(req, post);
    logger->log("db_put_handler", post.c_str());
    server->server_get_database()->database_put(post, "asdfqwerzxcvqwerasdfzxcvasdfqwerzxcv");
    server->server_send_reply_ok(req, post);
}


bool KyrinMasterServer::server_initialize()
{
    if (!server_initialize_kyrin_server_socket(reader_listen_fd, 7770, 10086)) {
        return false;
    }
    if (!server_initialize_kyrin_server_socket(processor_listen_fd, 7070, 10086)) {
        return false;
    }
    return true;
}

bool KyrinMasterServer::server_free()
{
    KyrinBaseServer::server_free();
    close(reader_listen_fd);
    close(processor_listen_fd);
    return true;
}

bool KyrinMasterServer::server_start()
{
    server_run(9);
    return true;
}

bool KyrinMasterServer::server_set_processor(evhttp *server, int thread_code)
{
    if (thread_code) {
        server_put_callback(server, "/get", db_get_handler, this);
        server_set_evhttp_accept_socket(server, reader_listen_fd);
    } else {
        server_put_callback(server, "/put", db_put_handler, this);
        server_set_evhttp_accept_socket(server, processor_listen_fd);
    }
    return true;
}

} /* server */
} /* kyrin */
