#include <unistd.h>
#include "kyrin_chunk_server.h"
#include "common/configs/kyrin_chunk_config.h"

namespace kyrin {
namespace server {

using namespace kyrin::common;

static void upload_chunk_file_handler(evhttp_request *req, void *arg)
{
    ((KyrinChunkServer *) arg)->get_upload_chunk_file_request_handler()->handle_request((KyrinChunkServer *)arg, req);
}

bool KyrinChunkServer::server_initialize(KyrinChunkGossiper *gossiper)
{
    if (gossiper == NULL) {
        return false;
    }
    m_gossiper = gossiper;

    configs::KyrinChunkConfig *config = new configs::KyrinChunkConfig();
    config->read_config_file((char *)"kyrinbox_config.json");
    if (!server_initialize_kyrin_server_socket(upload_chunk_file_fd,
         config->upload_chunk_file_port(),
         config->upload_chunk_file_backlog())) {
         delete config;
         return false;
    }

    upload_chunk_file_request_handler = new UploadChunkFileRequestHandler(m_gossiper);
    delete config;
    return true;
}

bool KyrinChunkServer::server_free()
{
    KyrinBaseServer::server_free();
    close(upload_chunk_file_fd);
    delete upload_chunk_file_request_handler;
    return true;
}

bool KyrinChunkServer::server_start()
{
    server_run(4);
    return true;
}

bool KyrinChunkServer::server_set_processor(evhttp *server, int thread_code)
{
    server_put_callback(server, "/UploadChunkFile", upload_chunk_file_handler, this);
    server_set_evhttp_accept_socket(server, upload_chunk_file_fd);
    return true;
}

} /* server */
} /* kyrin */
