#include "kyrin_base_server.h"
#include <iostream>

using namespace std;
namespace kyrin {
namespace server {

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

} /* server */
} /* kyrin */
