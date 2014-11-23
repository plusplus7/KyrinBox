#include <event2/buffer.h>
#include "kyrin_master_server.h"

namespace kyrin {
namespace server {

void cb1_handler(evhttp_request *req, void *arg) {
    
    evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, "cb1_get! %s\n", evhttp_request_get_uri(req));
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}

bool KyrinMasterServer::server_set_processor() {

    evhttp_set_cb(server_http_server, "/1", cb1_handler, NULL);

    return true;
}

} /* server */
} /* kyrin */
