#include "set_key_info_request_handler.h"
#include "common/kyrin_macros.h"
#include "common/crypto/kyrin_base64.h"

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

void SetFileKeyInfoRequestHandler::handler_request(KyrinChunkServer *server, evhttp_request *req)
{
    string reply = "Service denied...";
    string request_body = "";

    kyrin_todo();

    if (!server->server_get_postdata(req, request_body)) {
        reply = "Can't read post data...";
        break;
    }
    request_body = crypto::base64_decode(request_body);
    kyrinbox::api::SetFileKeyInfo info;
    info.ParseFromString(request_body);

    

    kyrin_done();

    server->server_send_reply_bad(req, reply);
}

} /* server */
} /* kyrin */
