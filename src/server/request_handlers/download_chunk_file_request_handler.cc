#include "download_chunk_file_request_handler.h"
#include <stdio.h>
#include "common/crypto/kyrin_base64.h"
#include "common/kyrin_macros.h"
#include "common/kyrin_constants.h"
#include "protobuf/upload_chunk_file.pb.h"

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

void DownloadChunkFileRequestHandler::handle_request(KyrinChunkServer *server, evhttp_request *req)
{
    string reply = "Service denied...";
    string request_body = "";

    kyrin_todo();

    if (!server->server_get_postdata(req, request_body)) {
        reply = "Can't read post data...";
        break;
    }

    request_body = crypto::base64_decode(request_body);
    kyrinbox::api::DownloadChunkFileRequest request;
    request.ParseFromString(request_body);

    char filepath[1024]; /* FIXME */

    sprintf(filepath, "./%s/%s.chunkfile", request.account().c_str(), request.file_name().c_str());
    FILE *fp = fopen(filepath, "rb+");
    if (fp == NULL) {
        reply = "failed to right";
        break;
    }

    string chunk = "";
    fseek(fp, request.offset(), SEEK_SET);

    char buff[constants::k_chunk_size+1];
    fread(buff, constants::k_chunk_size, 1, fp);
    reply.assign(buff, ftell(fp)-request.offset());
    fclose(fp);
    reply = crypto::base64_encode((unsigned char const*)reply.c_str(), reply.length());
    server->server_send_reply_ok(req, reply);
    return ;

    kyrin_done();
    server->server_send_reply_bad(req, reply);
}

} /* server */
} /* kyrin */
