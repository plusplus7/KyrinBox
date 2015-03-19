#include <unistd.h>
#include <vector>
#include "kyrin_chunk_gossiper.h"
#include "common/kyrin_log.h"
#include "common/kyrin_cluster.h"
#include "io/kyrin_http_client.h"
#include "common/crypto/kyrin_base64.h"

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;
using namespace kyrin::io;

static KyrinLog *logger = KyrinLog::get_instance();
static void *gossip_server_thread_func(void *arg)
{
    KyrinChunkGossiperServer *server = new KyrinChunkGossiperServer();
    KyrinChunkGossiper *gossiper = (KyrinChunkGossiper *)arg;
    KyrinCluster *cluster = KyrinCluster::get_instance();

    logger->log("gossip_server_thread_func", "starting server");
    if (!server->server_initialize(gossiper,
                                   cluster->get_chunk_config()->gossip_server_port(),
                                   cluster->get_chunk_config()->gossip_server_backlog())) {
       logger->log("gossip_thread_func", "start_server failed...");
       return NULL;
    }
    logger->log("gossip_server_thread_func", "server started");
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
    vector<uint32_t> seeds;
    vector<uint32_t> commons;
    seeds.clear();
    commons.clear();
    KyrinCluster *cluster = KyrinCluster::get_instance();
    cluster->get_chunk_status(seeds, commons);
    m_status.initialize(seeds, commons);
    return true;
}

void KyrinChunkGossiper::gossip_sync()
{
    KyrinCluster *cluster = KyrinCluster::get_instance();
    srand(time(NULL));
    while (true) {
        sleep(2);

        logger->log("gossip_sync", "gossip");
        vector< pair<uint32_t, int> > to_gossip;
        to_gossip.clear();
        to_gossip.push_back(make_pair(m_status.get_random_seed(), 1));
        to_gossip.push_back(make_pair(m_status.get_random_common(), 2));
        to_gossip.push_back(make_pair(m_status.get_random_dead_seed(), -1));
        to_gossip.push_back(make_pair(m_status.get_random_dead_common(), -2));

        for (uint32_t i=0; i<to_gossip.size(); i++) {
            if (to_gossip[i].first == 0) {
                continue;
            }
            uint32_t random_chunk = to_gossip[i].first;
            uint32_t retry = 0;
            string response;
            string to_post = "";
            kyrinbox::server::ChunkClusterGossipData send;
            send.set_kbid(cluster->get_kbid());
            send.set_host(cluster->get_chunk_ip());
            send.set_gossip_port(cluster->get_chunk_gossip_port());
            send.SerializeToString(&to_post);
            ChunkStatusConfig config;
            if (m_status.get_config(random_chunk, config) == false) {
                logger->log("gossip_sync", "Can't find random_chunk");
                continue;
            }
            to_post = crypto::base64_encode((unsigned char const*)to_post.c_str(), to_post.length());

            while (!KyrinHttpClient::make_request_post(config.host.c_str(),
                                                       config.gossip_port,
                                                       "/get_status",
                                                       response,
                                                       to_post)
                   && retry < 3) {
                retry++;
            }
            /* FIXME: Not best practise */
            if (retry == 3 && to_gossip[i].second > 0) {
                m_status.set_dead(to_gossip[i].first, to_gossip[i].second);
            } else if (retry < 3 && to_gossip[i].second < 0) {
                m_status.set_alive(to_gossip[i].first, to_gossip[i].second);
            }
            if (retry == 3) {
                logger->log("gossip_sync", "detect failure");
                continue;
            }
            cout << " ====== my status ======" << endl;
            m_status.show();
            cout << " ====== my status end ======" << endl;
            response = crypto::base64_decode(response);
            /* FIXME: waste of resource*/
            KyrinChunkGossiperStatus t_status;
            if (t_status.from_string(response) == false) {
                logger->log("gossip_sync", "t_status from_string failed");
                continue;
            }
            if (t_status.get_timestamp() > m_status.get_timestamp()) {
                if (!m_status.from_string(response)) {
                    logger->log("gossip_sync", "Assign m_status failed");
                }
            }
        }
    }
}

} /* server */
} /* kyrin */
