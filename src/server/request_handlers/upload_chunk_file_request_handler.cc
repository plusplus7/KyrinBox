#include "upload_chunk_file_request_handler.h"

namespace kyrin {
namespace server {

using namespace std;

void UploadChunkFileRequestHandler::handle_request(KyrinChunkServer *server, evhttp_request *req)
{
    string reply = "233";
    server->server_send_reply_ok(req, reply);
}

} /* server */
} /* kyrin */
