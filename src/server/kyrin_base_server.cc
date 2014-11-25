#include "kyrin_base_server.h"
#include "common/kyrin_log.h"
#include <iostream>
#include <string>

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;
using namespace kyrin::io;

static KyrinLog *logger = KyrinLog::get_instance();
bool KyrinBaseServer::server_initialize(const char *address, int port) {

    server_event_base = event_base_new();
    if (server_event_base == NULL) {
        logger->log("server_initialize", "server_event_base new failed");
        return false;
    }
    server_http_server = evhttp_new(server_event_base);
    if (server_event_base == NULL) {
        logger->log("server_initialize", "server_event_base new failed");
        return false;
    }
    if (evhttp_bind_socket(server_http_server, address, port) == -1) {
        logger->log("server_initialize", "socket bind failed");
        return false;
    }

    server_database_wrapper = new KyrinDatabaseWrapper();
    return true;
}

bool KyrinBaseServer::server_run() {
    event_base_dispatch(server_event_base); 
    return true;
}

bool KyrinBaseServer::server_free() {
    evhttp_free(server_http_server);
    delete server_database_wrapper;
    return true;
}

bool KyrinBaseServer::server_put_callback(const char *path, void(*cb)(struct evhttp_request *, void *), void *cb_arg) {
    evhttp_set_cb(server_http_server, path, cb, cb_arg);
    return true;
}

bool KyrinBaseServer::server_send_reply_ok(evhttp_request *req, string &msg) {
    evbuffer *buf = evbuffer_new();
    if (buf == NULL) {
        logger->log("server_send_reply_ok", "new buf failed");
        return false;
    }
    evbuffer_add_printf(buf, "%s", msg.c_str());
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
    return true;
}

bool KyrinBaseServer::server_get_postdata(evhttp_request *req, string &post_data) {
    size_t post_size = evbuffer_get_length(evhttp_request_get_input_buffer(req));
    if (/* likely */post_size > 0) {
        char _buf[1024]; /* FIXME */
        memcpy(_buf, evbuffer_pullup(evhttp_request_get_input_buffer(req), -1), post_size);
        post_data.assign(_buf, post_size);
        return true;
    }
    return false;
}

KyrinDatabaseWrapper* KyrinBaseServer::server_get_database()
{
    return server_database_wrapper;
}

} /* server */
} /* kyrin */
