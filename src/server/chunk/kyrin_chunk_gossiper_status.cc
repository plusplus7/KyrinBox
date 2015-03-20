#include "kyrin_chunk_gossiper_status.h"
#include "common/kyrin_cluster.h"

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

void KyrinChunkGossiperStatus::initialize(vector<uint32_t> &seeds, vector<uint32_t> &commons, uint64_t timestamp)
{
    KyrinCluster *cluster = KyrinCluster::get_instance();
    m_status_lock.lock();
    m_configs.clear();
    m_seeds.assign(seeds.begin(), seeds.end());
    m_commons.assign(commons.begin(), commons.end());
    if (timestamp) {
        m_timestamp = timestamp;
    } else {
        m_timestamp = time(NULL);
    }
    srand(time(NULL));
    for (uint32_t i=0; i<seeds.size(); i++) {
        m_configs[seeds[i]] = ChunkStatusConfig(cluster->get_chunk_ip(seeds[i]), cluster->get_chunk_gossip_port(seeds[i]));
    }
    for (uint32_t i=0; i<commons.size(); i++) {
        m_configs[commons[i]] = ChunkStatusConfig(cluster->get_chunk_ip(commons[i]), cluster->get_chunk_gossip_port(commons[i]));
    }
    m_status_lock.unlock();
}

bool KyrinChunkGossiperStatus::check_commons(kyrinbox::server::ChunkClusterGossipData &data)
{
    bool ret = false;
    m_status_lock.lock();
    uint32_t kbid = data.kbid();
    if (m_configs.count(kbid) == 0) {
        m_commons.push_back(kbid);
        m_configs[kbid] = ChunkStatusConfig(data.host(), data.gossip_port());
        m_timestamp = time(NULL);
        ret = true;
    }
    m_status_lock.unlock();
    return ret;
}

bool KyrinChunkGossiperStatus::get_config(uint32_t kbid, ChunkStatusConfig &config)
{
    bool ret = false;
    m_status_lock.lock();
    if (m_configs.count(kbid)) {
        config = m_configs[kbid];
        ret = true;
    }
    m_status_lock.unlock();
    return ret;
}

bool KyrinChunkGossiperStatus::to_protobuf(kyrinbox::server::ChunkClusterStatus &status)
{
    status.clear_seeds();
    status.clear_commons();
    status.clear_dead_seeds();
    status.clear_dead_commons();
    status.clear_datas();

    m_status_lock.lock();
    for (uint32_t i=0; i<m_seeds.size(); i++) {
        kyrinbox::server::ChunkStatus *c_status = status.add_seeds();
        c_status->set_kbid(m_seeds[i]);
    }

    for (uint32_t i=0; i<m_commons.size(); i++) {
        kyrinbox::server::ChunkStatus *c_status = status.add_commons();
        c_status->set_kbid(m_commons[i]);
    }

    for (uint32_t i=0; i<m_dead_seeds.size(); i++) {
        kyrinbox::server::ChunkStatus *c_status = status.add_dead_seeds();
        c_status->set_kbid(m_dead_seeds[i]);
    }

    for (uint32_t i=0; i<m_dead_commons.size(); i++) {
        kyrinbox::server::ChunkStatus *c_status = status.add_dead_commons();
        c_status->set_kbid(m_dead_commons[i]);
    }

    for (map<uint32_t, ChunkStatusConfig>::iterator iter=m_configs.begin(); iter!=m_configs.end(); ++iter) {
        kyrinbox::server::ChunkClusterGossipData *c_data = status.add_datas();
        c_data->set_kbid(iter->first);
        c_data->set_host((iter->second).host);
        c_data->set_gossip_port((iter->second).gossip_port);
    }
    status.set_timestamp(m_timestamp);

    m_status_lock.unlock();
    return true;
}

bool KyrinChunkGossiperStatus::to_string(string *status_str)
{
    kyrinbox::server::ChunkClusterStatus status;
    to_protobuf(status);
    return status.SerializeToString(status_str);
}

bool KyrinChunkGossiperStatus::from_string(string &status_str)
{
    kyrinbox::server::ChunkClusterStatus status;
    if (!status.ParseFromString(status_str)) {
        return false;
    }

    m_status_lock.lock();
    m_seeds.clear();
    m_commons.clear();
    m_dead_seeds.clear();
    m_dead_commons.clear();
    m_configs.clear();

    for (uint32_t i=0; i<status.seeds_size(); i++) {
        m_seeds.push_back(status.seeds(i).kbid());
    }

    for (uint32_t i=0; i<status.commons_size(); i++) {
        m_commons.push_back(status.commons(i).kbid());
    }

    for (uint32_t i=0; i<status.dead_seeds_size(); i++) {
        m_dead_seeds.push_back(status.dead_seeds(i).kbid());
    }

    for (uint32_t i=0; i<status.dead_commons_size(); i++) {
        m_dead_commons.push_back(status.dead_commons(i).kbid());
    }

    for (uint32_t i=0; i<status.datas_size(); i++) {
        m_configs[status.datas(i).kbid()] = ChunkStatusConfig(status.datas(i).host(), status.datas(i).gossip_port());
    }
    m_timestamp = status.timestamp();
    m_status_lock.unlock();
    return true;
}

void KyrinChunkGossiperStatus::set_dead(uint32_t kbid, int flag)
{
    if (flag == 1) {
        set_item(m_seeds, m_dead_seeds, kbid);
    } else if (flag == 2) {
        set_item(m_commons, m_dead_commons, kbid);
    }
}

void KyrinChunkGossiperStatus::set_alive(uint32_t kbid, int flag)
{
    if (flag == -1) {
        set_item(m_dead_seeds, m_seeds, kbid);
    } else if (flag == -2) {
        set_item(m_dead_commons, m_commons, kbid);
    }
}

void KyrinChunkGossiperStatus::set_item(vector<uint32_t> &a, vector<uint32_t> &b, uint32_t kbid)
{
    m_status_lock.lock();
    m_timestamp = time(NULL);
    for (vector<uint32_t>::iterator iter=a.begin(); iter!=a.end(); iter++) {
        if (kbid == *iter) {
            a.erase(iter);
            b.push_back(kbid);
            m_status_lock.unlock();
            return ;
        }
    }
    m_status_lock.unlock();
}

} /* server */
} /* kyrin */
