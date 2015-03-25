#ifndef KYRINBOX_SRC_SERVER_KYRIN_CHUNK_SERVER_H_
#define KYRINBOX_SRC_SERVER_KYRIN_CHUNK_SERVER_H_

#include "server/base/kyrin_base_server.h"
#include "io/kyrin_database_wrapper.h"
#include "server/chunk/kyrin_chunk_gossiper.h"
#include "server/request_handlers/upload_chunk_file_request_handler.h"
#include "server/request_handlers/set_file_key_info_request_handler.h"

namespace kyrin {
namespace server {

class UploadChunkFileRequestHandler;
class SetFileKeyInfoRequestHandler;

class KyrinChunkServer : public KyrinBaseServer {
public:
    bool server_initialize(kyrin::server::KyrinChunkGossiper *gossiper);
    bool server_start();
    bool server_free();
    bool server_set_processor(evhttp *server, int thread_code);

    UploadChunkFileRequestHandler *get_upload_chunk_file_request_handler() {
        return upload_chunk_file_request_handler;
    }

    SetFileKeyInfoRequestHandler *get_set_file_key_info_request_handler() {
        return set_file_key_info_request_handler;
    }

private:
    int upload_chunk_file_fd;
    int download_chunk_file_fd;
    int set_file_key_info_fd;
    int get_file_key_info_fd;
    KyrinChunkGossiper *m_gossiper;
    kyrin::io::KyrinDatabaseWrapper *m_keyinfo_db;
    UploadChunkFileRequestHandler *upload_chunk_file_request_handler;
    SetFileKeyInfoRequestHandler *set_file_key_info_request_handler;
};

} /* server */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_KYRIN_CHUNK_SERVER_H_ */
