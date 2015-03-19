#ifndef KYRINBOX_SRC_COMMON_KYRIN_CLUSTER_H_
#define KYRINBOX_SRC_COMMON_KYRIN_CLUSTER_H_

#include <string>
#include <vector>
#include <utility>
#include "configs/kyrin_master_config.h"
#include "configs/kyrin_chunk_config.h"

namespace kyrin {
namespace common {

enum KyrinServerType{
    k_type_master = 1,
    k_type_slavenode = 2,
    k_type_chunk = 3
};

class KyrinCluster {
public:
    static KyrinCluster *get_instance();

    bool read_config_file(const char *filename);

    configs::KyrinMasterConfig *get_master_config(uint32_t kbid = 0) {
        if (!kbid)
            kbid = m_kbid;
        return &m_master_configs[kbid-1];
    }

    std::pair<const char *, int> get_random_chunk_seed_for_gossip() {
        int rnd = 3001+rand()%m_chunk_seed_count;
        return std::make_pair(get_chunk_ip(rnd), get_chunk_gossip_port(rnd));
    }
    const char *get_chunk_ip(uint32_t kbid = 0) {
        if (!kbid)
            kbid = m_kbid;
        return m_chunk_configs[kbid-3001].machine_address().c_str();
    }

    uint32_t get_chunk_gossip_port(uint32_t kbid = 0) {
        if (!kbid)
            kbid = m_kbid;
        return m_chunk_configs[kbid-3001].gossip_server_port();
    }

    configs::KyrinChunkConfig *get_chunk_config(uint32_t kbid = 0) {
        if (!kbid)
            kbid = m_kbid;
        return &m_chunk_configs[kbid-3001];
    }

    void get_chunk_status(std::vector<uint32_t> &seeds, std::vector<uint32_t> &commons) {
        for (uint32_t i=0; i<m_chunk_configs.size(); i++) {
            if (m_chunk_configs[i].role() == "Seed") {
                seeds.push_back(i+3001);
            } else {
                commons.push_back(i+3001);
            }
        }
    }

    const char *get_master_ip(uint32_t kbid = 0) {
        if (!kbid)
            kbid = m_kbid;
        return m_master_configs[kbid-1].machine_address().c_str();
    }

    uint32_t get_master_sentinel_port(uint32_t kbid = 0) {
        if (!kbid)
            kbid = m_kbid;
        return m_master_configs[kbid-1].sentinel_port();
    }

    uint32_t get_master_confirm_oplog_port(uint32_t kbid = 0) {
        if (!kbid)
            kbid = m_kbid;
        return m_master_configs[kbid-1].confirm_oplog_port();
    }

    uint32_t get_master_get_oplog_port(uint32_t kbid = 0) {
        if (!kbid)
            kbid = m_kbid;
        return m_master_configs[kbid-1].get_oplog_port();
    }

    uint32_t get_kbid() {
        return m_kbid;
    }

    uint32_t get_master_count() {
        return m_master_server_count;
    }

private:
    KyrinCluster() {
        read_config_file(constants::k_config_filepath);
        srand(time(NULL));
    }
    virtual ~KyrinCluster() {}

    static KyrinCluster *m_kyrin_cluster;
    KyrinServerType m_server_type;
    uint32_t m_kbid;
    uint32_t m_master_server_count;
    uint32_t m_chunk_seed_count;
    std::vector<configs::KyrinMasterConfig> m_master_configs;
    std::vector<configs::KyrinChunkConfig> m_chunk_configs;
};

} /* common */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_KYRIN_CLUSTER_H_ */
