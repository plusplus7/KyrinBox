#include <unistd.h>
#include "kyrin_chunk_gossiper.h"
#include "common/kyrin_log.h"
#include "common/kyrin_cluster.h"

namespace kyrin {
namespace server {

using namespace kyrin::common;

static KyrinLog *logger = KyrinLog::get_instance();
static void *gossip_server_thread_func(void *arg)
{
    logger->log("gossip_server_thread_func", "starting server");
    KyrinChunkGossiperServer *server = new KyrinChunkGossiperServer();
    logger->log("gossip_server_thread_func", "starting server");
    KyrinChunkGossiper *gossiper = (KyrinChunkGossiper *)arg;
    logger->log("gossip_server_thread_func", "starting server");
    KyrinCluster *cluster = KyrinCluster::get_instance();

    logger->log("gossip_server_thread_func", "starting server");
    if (!server->server_initialize(gossiper,
                                   cluster->get_chunk_config()->gossip_server_port(),
                                   cluster->get_chunk_config()->gossip_server_backlog())) {
       logger->log("gossip_thread_func", "start_server failed...");
       return NULL;
    }
    server->server_start();
    server->server_close();

    delete server;
    return NULL;
}

static void *gossip_sync_func(void *arg)
{
    ((KyrinChunkGossiper *) arg)->gossip_sync();
    return NULL;
}

bool KyrinChunkGossiper::start_gossip()
{
    pthread_t gossip_sync_t;
    pthread_t gossip_server_t;
    logger->log("start_gossip", "starting server");
    if (pthread_create(&gossip_server_t, NULL, gossip_server_thread_func, (void *)this)) {
        return false;
    }
    if (pthread_create(&gossip_sync_t, NULL, gossip_sync_func, (void *)this)) {
        return false;
    }
    return true;
}

void KyrinChunkGossiper::gossip_sync()
{
    while (true) {
        logger->log("gossip_sync", "aha");
        sleep(2);
    }

}

} /* server */
} /* kyrin */
