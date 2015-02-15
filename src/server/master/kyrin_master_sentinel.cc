#include "kyrin_master_sentinel.h"
#include "io/kyrin_http_client.h"
#include "server/base/kyrin_base_server.h"
#include "kyrin_master_sentinel_server.h"
#include "common/kyrin_log.h"
#include "common/kyrin_cluster.h"
#include <unistd.h>
#include <set>
#include <fstream>
#include <iostream>
#include <utility>

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;
using namespace kyrin::io;

static KyrinLog *logger = KyrinLog::get_instance();
static void *sentinel_thread_func(void *arg)
{
    KyrinMasterSentinel *sentinel = (KyrinMasterSentinel *) arg;
    KyrinMasterSentinelServer *server = new KyrinMasterSentinelServer();

    if (!server->server_initialize(sentinel, KyrinCluster::get_instance()->get_master_sentinel_port(), 128*8)) {
        logger->log("sentinel_thread_func", "start_server failed...");
        return NULL;
    }
    server->server_start();
    server->server_close();

    delete server;
    return NULL;
}

static bool check_response(string &response)
{
    if (response != "")
        return true;
    return false;
}

static void *sentinel_sync_func(void *arg)
{
    KyrinMasterSentinel *sentinel = (KyrinMasterSentinel *) arg;
    KyrinCluster *cluster = KyrinCluster::get_instance();
    //TODO: Add control to close server
    bool is_running = true;
    int leader = -1;
    while (is_running) {
        KyrinMasterStatus status;
        sentinel->get_status(status);
        switch (status) {
            case k_status_leader:
                break;
            case k_status_follower:
                {
                    string response;
                    int retry = 0;
                    while (!KyrinHttpClient::make_request_get(cluster->get_master_ip(leader),
                                                              cluster->get_master_sentinel_port(leader),
                                                              "/get_status",
                                                              response)
                          && retry != 3) {
                          retry++;
                    }
                    if (retry == 3) {
                        sentinel->set_status(k_status_consensus);
                        sentinel->set_vote(cluster->get_kbid(), true);
                    }
                }
                break;
            case k_status_consensus:
                {
                    uint32_t master_count = cluster->get_master_count();
                    vector< pair<uint64_t, uint64_t> > votes(master_count+1, make_pair(0, 0));
                    vector<bool> voted(master_count, false);
                    while (true) {
                        string me_vote;
                        sentinel->get_vote(me_vote);
                        for (uint32_t i=1; i <= master_count; i++) {

                            string response;
                            KyrinHttpClient::make_request_post(cluster->get_master_ip(i),
                                                               cluster->get_master_sentinel_port(i),
                                                               "/get_vote",
                                                               response,
                                                               me_vote);
                            if (check_response(response)) {
                                uint64_t epoch, vote;
                                sscanf(response.c_str(), "%llu %llu", &epoch, &vote);

                                cout<<epoch<<" "<<vote<<endl;

                                if (epoch != sentinel->get_epoch()) {
                                    while (epoch > sentinel->get_epoch()) {
                                        sentinel->set_vote(cluster->get_kbid(), true);
                                    }
                                    break;
                                } else {
                                    votes[i].first = epoch;
                                    votes[i].second = vote;
                                    voted[i] = true;
                                }
                            }
                        }

                        vector<int> vote_result(master_count+1, 0);
                        for (uint32_t i=1; i < votes.size(); i++) {
                            if (voted[i]) {
                                vote_result[votes[i].second]++;
                            }
                        }

                        leader = -1;
                        int max_candidate = -1;
                        for (uint32_t i=1; i < vote_result.size(); i++) {
                            if (vote_result[i] >= (master_count>>1)+1) {
                                leader = i;
                            }
                            if (vote_result[i]) {
                                max_candidate = i;
                            }
                        }

                        if (max_candidate == -1) {
                            cout << "unexpected candidate" << endl;
                            break;
                        }
                        if (leader == -1) {
                            sentinel->set_vote(max_candidate);
                        } else {
                            sentinel->set_vote(leader);
                            if (leader != cluster->get_kbid()) {
                                cout << "status: follower" << endl;
                                sentinel->set_status(k_status_follower);
                            } else {
                                cout << "status: leader" << endl;
                                sentinel->set_status(k_status_leader);
                            }
                            cout << "leader: " << leader << endl;
                            break;
                        }
                        sleep(2);
                    }
                }
                break;
            default:
                is_running = false;
                break;
        }
        sleep(2);
    }
    return NULL;
}

bool KyrinMasterSentinel::start_sentinel()
{
    m_epoch = 0;
    m_leader = KyrinCluster::get_instance()->get_kbid();
    m_status = k_status_consensus;
    pthread_t sentinel_thread_t;
    pthread_t sentinel_sync_t;

    if (pthread_create(&sentinel_thread_t, NULL, sentinel_thread_func, (void *)this)) {
        return false;    
    }
    if (pthread_create(&sentinel_sync_t, NULL, sentinel_sync_func, (void *)this)) {
        return false;
    }
    return true;
}

bool KyrinMasterSentinel::get_vote(string &message)
{
    m_vote_lock.lock();

    char msg[128];
    sprintf(msg, "%llu %llu", m_epoch, m_leader);
    message.assign(msg, strlen(msg));

    m_vote_lock.unlock();
    return true;
}

bool KyrinMasterSentinel::set_vote(uint64_t vote, bool next_epoch)
{
    m_vote_lock.lock();
    if (next_epoch) {
        m_epoch++;
    }
    m_leader = vote;
    m_vote_lock.unlock();
    return true;
}

bool KyrinMasterSentinel::get_status(KyrinMasterStatus &message)
{
    m_status_lock.lock();
    message = m_status;
    m_status_lock.unlock();
    return true;
}

bool KyrinMasterSentinel::set_status(KyrinMasterStatus status)
{
    m_status_lock.lock();
    m_status = status;
    m_status_lock.unlock();
    return true;
}

} /* server */
} /* kyrin */
