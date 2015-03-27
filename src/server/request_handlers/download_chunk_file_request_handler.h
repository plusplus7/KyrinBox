#ifndef KYRINBOX_SRC_SERVER_REQUEST_HANDLER_DOWNLOAD_CHUNK_FILE_REQUEST_HANDLER_H_
#define KYRINBOX_SRC_SERVER_REQUEST_HANDLER_DOWNLOAD_CHUNK_FILE_REQUEST_HANDLER_H_

#include "server/chunk/kyrin_chunk_server.h"
#include "server/chunk/kyrin_chunk_gossiper.h"

namespace kyrin {
namespace server {

class KyrinChunkServer;

class DownloadChunkFileRequestHandler {
public:
    DownloadChunkFileRequestHandler() {}
    ~DownloadChunkFileRequestHandler() {}

    void handle_request(KyrinChunkServer *server, evhttp_request *req);

private:
};

} /* server */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_REQUEST_HANDLER_DOWNLOAD_CHUNK_FILE_REQUEST_HANDLER_H_ */
