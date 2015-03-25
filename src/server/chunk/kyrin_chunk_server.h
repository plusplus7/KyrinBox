#ifndef KYRINBOX_SRC_SERVER_KYRIN_CHUNK_SERVER_H_
#define KYRINBOX_SRC_SERVER_KYRIN_CHUNK_SERVER_H_

#include "server/base/kyrin_base_server.h"
#include "server/chunk/kyrin_chunk_gossiper.h"
#include "server/request_handlers/upload_chunk_file_request_handler.h"

namespace kyrin {
namespace server {

class UploadChunkFileRequestHandler;

class KyrinChunkServer : public KyrinBaseServer {
public:
    bool server_initialize(kyrin::server::KyrinChunkGossiper *gossiper);
    bool server_start();
    bool server_free();
    bool server_set_processor(evhttp *server, int thread_code);

    UploadChunkFileRequestHandler *get_upload_chunk_file_request_handler() {
        return upload_chunk_file_request_handler;
    }

private:
    int upload_chunk_file_fd;
    KyrinChunkGossiper *m_gossiper;
    UploadChunkFileRequestHandler *upload_chunk_file_request_handler;
};

} /* server */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_KYRIN_CHUNK_SERVER_H_ */
