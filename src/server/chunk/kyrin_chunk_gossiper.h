#ifndef KYRINBOX_SRC_SERVER_CHUNK_KYRIN_CHUNK_GOSSIPER_H_
#define KYRINBOX_SRC_SERVER_CHUNK_KYRIN_CHUNK_GOSSIPER_H_

#include "kyrin_chunk_gossiper_server.h"

namespace kyrin {
namespace server {

class KyrinChunkGossiper {
public:
    bool start_gossip();
    void gossip_sync();

private:
};

}
}

#endif /* end of include guard: KYRINBOX_SRC_SERVER_CHUNK_KYRIN_CHUNK_GOSSIPER_H_ */
