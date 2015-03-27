#include "upload_chunk_file_request_handler.h"
#include <stdio.h>
#include "common/crypto/kyrin_base64.h"
#include "common/kyrin_macros.h"
#include "common/kyrin_constants.h"
#include "protobuf/upload_chunk_file.pb.h"

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

void UploadChunkFileRequestHandler::handle_request(KyrinChunkServer *server, evhttp_request *req)
{
    string reply = "Service denied...";
    string request_body = "";

    kyrin_todo();

    if (!server->server_get_postdata(req, request_body)) {
        reply = "Can't read post data...";
        break;
    }

    request_body = crypto::base64_decode(request_body);
    kyrinbox::api::UploadChunkFileRequest request;
    request.ParseFromString(request_body);

    char filepath[1024]; /* FIXME */

    sprintf(filepath, "./%s/%s.chunkfile", request.account().c_str(), request.file_name().c_str());
    FILE *fp = fopen(filepath, "rb+");
    if (fp == NULL) {
        reply = "failed to right";
        break;
    }

    string chunk = crypto::base64_decode(request.chunk());
    fseek(fp, request.offset(), SEEK_SET);
    int num_write = fwrite(chunk.c_str(), chunk.length(), 1, fp);
    fclose(fp);
    reply = "OK";
    server->server_send_reply_ok(req, reply);
    return ;

    kyrin_done();
    server->server_send_reply_bad(req, reply);
}

} /* server */
} /* kyrin */
