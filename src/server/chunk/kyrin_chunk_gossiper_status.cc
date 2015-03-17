#include "kyrin_chunk_gossiper_status.h"

namespace kyrin {
namespace server {

using namespace std;

void KyrinChunkGossiperStatus::initialize(vector<uint32_t> &seeds, vector<uint32_t> &commons, uint64_t timestamp)
{
    m_seeds.assign(seeds.begin(), seeds.end());
    m_commons.assign(commons.begin(), commons.end());
    if (timestamp) {
        m_timestamp = timestamp;
    } else {
        m_timestamp = time(NULL);
    }
    srand(time(NULL));
}

bool KyrinChunkGossiperStatus::to_protobuf(kyrinbox::server::ChunkClusterStatus &status)
{
    status.clear_seeds();
    status.clear_commons();
    status.clear_dead_seeds();
    status.clear_dead_commons();

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
    status.set_timestamp(m_timestamp);
    
    m_status_lock.unlock();
    return true;
}

bool KyrinChunkGossiperStatus::to_string(string &status_str)
{
    kyrinbox::server::ChunkClusterStatus status;
    to_protobuf(status);
    return status.SerializeToString(&status_str);
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
