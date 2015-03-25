#ifndef KYRINBOX_SRC_SERVER_REQUEST_HANDLER_UPLOAD_CHUNK_FILE_REQUEST_HANDLER_H_
#define KYRINBOX_SRC_SERVER_REQUEST_HANDLER_UPLOAD_CHUNK_FILE_REQUEST_HANDLER_H_

#include "server/chunk/kyrin_chunk_server.h"
#include "server/chunk/kyrin_chunk_gossiper.h"

namespace kyrin {
namespace server {

class KyrinChunkServer;

class UploadChunkFileRequestHandler {
public:
    UploadChunkFileRequestHandler(kyrin::server::KyrinChunkGossiper *gossiper)
                                  : m_gossiper(gossiper) {}
    ~UploadChunkFileRequestHandler() {}

    void handle_request(KyrinChunkServer *server, evhttp_request *req);

private:
    kyrin::server::KyrinChunkGossiper *m_gossiper;
};

} /* server */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_REQUEST_HANDLER_UPLOAD_CHUNK_FILE_REQUEST_HANDLER_H_ */
