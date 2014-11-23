#include "kyrin_base_server.h"
#include <iostream>
#include <string>

namespace kyrin {
namespace server {

using namespace std;

bool KyrinBaseServer::server_initialize(const char *address, int port) {

    server_event_base = event_base_new();
    if (server_event_base == NULL) {
        cout<<"server_initialize: server_event_base new failed..."<<endl;
        return false;
    }
    server_http_server = evhttp_new(server_event_base);
    if (server_event_base == NULL) {
        cout<<"server_initialize: server_http_server new failed..."<<endl;
        return false;
    }
    if (evhttp_bind_socket(server_http_server, address, port) == -1) {
        cout<<"server_initialize: evhttp_bind_socket failed..."<<endl;
        return false;
    }
    return true;
}

bool KyrinBaseServer::server_run() {
    event_base_dispatch(server_event_base); 
}

bool KyrinBaseServer::server_free() {
    evhttp_free(server_http_server);
}

bool KyrinBaseServer::server_put_callback(const char *path, void(*cb)(struct evhttp_request *, void *), void *cb_arg) {
    evhttp_set_cb(server_http_server, path, cb, cb_arg);
}

bool KyrinBaseServer::server_send_reply_ok(evhttp_request *req, string &msg) {
    evbuffer *buf = evbuffer_new();
    if (buf == NULL) {
        cout<<"server_send_reply_ok: new buf failed"<<endl;
        return false;
    }
    evbuffer_add_printf(buf, msg.c_str());
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}

} /* server */
} /* kyrin */
