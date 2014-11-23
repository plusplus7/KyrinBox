#include <event2/buffer.h>
#include <string>
#include "kyrin_master_server.h"

namespace kyrin {
namespace server {

using namespace std;

static void cb1_handler(evhttp_request *req, void *arg) {
    
    string reply = "CB1 Get you";
    KyrinBaseServer *server = (KyrinBaseServer *) arg;
    server->server_send_reply_ok(req, reply);
}

bool KyrinMasterServer::server_set_processor() {

    server_put_callback("/1", cb1_handler, this);
    return true;
}

} /* server */
} /* kyrin */
