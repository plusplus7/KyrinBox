#ifndef KYRINBOX_SRC_SERVER_CHUNK_KYRIN_CHUNK_GOSSIPER_STATUS_H_
#define KYRINBOX_SRC_SERVER_CHUNK_KYRIN_CHUNK_GOSSIPER_STATUS_H_

#include "protobuf/chunk_cluster_status.pb.h"
#include "common/kyrin_lock.h"
#include <utility>
#include <vector>

namespace kyrin {
namespace server {

struct ChunkStatusConfig {
    std::string host;
    uint32_t gossip_port;
    ChunkStatusConfig(std::string th = "", uint32_t gp = 0):host(th), gossip_port(gp) {}
    ~ChunkStatusConfig(){}
};

class KyrinChunkGossiperStatus {
public:
    void initialize(std::vector<uint32_t> &seeds, std::vector<uint32_t> &commons, uint64_t timestamp = 0);
    bool to_protobuf(kyrinbox::server::ChunkClusterStatus &status);
    bool to_string(std::string *status_str);
    bool from_string(std::string &status_str);
    void set_dead(uint32_t kbid, int flag);
    void set_alive(uint32_t kbid, int flag);
    bool check_commons(kyrinbox::server::ChunkClusterGossipData &data);
    bool get_config(uint32_t kbid, ChunkStatusConfig &config);
    uint64_t get_timestamp() {
        return m_timestamp;
    }
    bool get_random_alive_3(std::vector< std::pair<uint32_t, ChunkStatusConfig> > &res) {
        std::vector<uint32_t> v;
        v.clear();
        res.clear();
        int n = m_seeds.size()+m_commons.size();
        if (n < 3) {
            return false;
        }
        for (uint32_t i=0; i<3;) {
            uint32_t tk = rand()%n;
            if (tk >= m_seeds.size())
                tk = m_commons[tk-m_seeds.size()];
            else
                tk = m_seeds[tk];
            for (uint32_t j=0; ;j++) {
                if (j == i) {
                    res.push_back(std::make_pair(tk, m_configs[tk]));
                    v.push_back(tk);
                    i++;
                    break;
                }
                if (tk == v[j]) {
                    break;
                }
            }
        }
        return true;
    }
    uint32_t get_random_seed() {
        if (m_seeds.size() == 0)
            return 0;
        return get_random(m_seeds);
    }
    uint32_t get_random_common() {
        if (m_commons.size() == 0)
            return 0;
        return get_random(m_commons);
    }
    uint32_t get_random_dead_seed() {
        if (m_dead_seeds.size() == 0)
            return 0;
        return get_random(m_dead_seeds);
    }
    uint32_t get_random_dead_common() {
        if (m_dead_commons.size() == 0)
            return 0;
        return get_random(m_dead_commons);
    }
    /* FIXME: remove me when going to merge */
    /* void show() {
        std::cout << "timestamp:" << " " << m_timestamp << std::endl;
        std::cout << "seeds:";
        for (auto i : m_seeds) {
            std::cout << " " << i;
        }
        std::cout<<std::endl;
        std::cout << "commons:";
        for (auto i : m_commons) {
            std::cout << " " << i;
        }
        std::cout<<std::endl;
        std::cout << "dead_seeds:";
        for (auto i : m_dead_seeds) {
            std::cout << " " << i;
        }
        std::cout<<std::endl;
        std::cout << "dead_commons:";
        for (auto i : m_dead_commons) {
            std::cout << " " << i;
        }
        std::cout<<std::endl;
    } */

private:
    void set_item(std::vector<uint32_t> &a, std::vector<uint32_t> &b, uint32_t kbid);
    uint32_t get_random(std::vector<uint32_t> &a) {
        return a[rand()%a.size()];
    }

    kyrin::common::KyrinMutex m_status_lock;
    uint64_t m_timestamp;
    std::map<uint32_t, ChunkStatusConfig> m_configs;
    std::vector<uint32_t> m_seeds;
    std::vector<uint32_t> m_commons;
    std::vector<uint32_t> m_dead_seeds;
    std::vector<uint32_t> m_dead_commons;
};

} /* server */
} /* kyrin */
#endif /* end of include guard: KYRINBOX_SRC_SERVER_CHUNK_KYRIN_CHUNK_GOSSIPER_STATUS_H_ */
