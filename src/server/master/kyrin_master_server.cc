#include <event2/buffer.h>
#include <string>
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
bool KyrinMasterServer::server_set_processor(evhttp *server, int thread_code)
{
    if (!thread_code) {
        server_put_callback(server, "/get", db_get_handler, this);
    } else {
        server_put_callback(server, "/put", db_put_handler, this);
    }
    return true;
}

} /* server */
} /* kyrin */
