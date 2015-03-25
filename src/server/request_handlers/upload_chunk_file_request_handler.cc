#include "upload_chunk_file_request_handler.h"
#include "common/kyrin_macros.h"

namespace kyrin {
namespace server {

using namespace std;

void UploadChunkFileRequestHandler::handle_request(KyrinChunkServer *server, evhttp_request *req)
{
    string reply = "Service denied...";
    string request_body = "";

    kyrin_todo();

    if (!server->server_get_postdata(req, request_body)) {
        reply = "Can't read post data...";
        break;
    }

    kyrin_done();
    server->server_send_reply_ok(req, reply);
}

} /* server */
} /* kyrin */
