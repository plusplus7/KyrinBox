#ifndef KYRINBOX_SRC_SERVER_CHUNK_KYRIN_CHUNK_GOSSIPER_SERVER_H_
#define KYRINBOX_SRC_SERVER_CHUNK_KYRIN_CHUNK_GOSSIPER_SERVER_H_

#include "server/base/kyrin_base_server.h"
#include "kyrin_chunk_gossiper.h"

namespace kyrin {
namespace server {

class KyrinChunkGossiper;

class KyrinChunkGossiperServer : public KyrinBaseServer {
public:
    bool server_initialize(KyrinChunkGossiper *gossiper, int port, int backlog);
    bool server_set_processor(evhttp *server, int thread_code);
    bool server_start();
    bool server_close();

    bool handle_request(evhttp_request *req);

private:
    KyrinChunkGossiper *m_gossiper;
    int server_listen_fd;
};

} /* server */
} /* kyrin */
#endif /* end of include guard: KYRINBOX_SRC_SERVER_CHUNK_KYRIN_CHUNK_GOSSIPER_SERVER_H_ */
