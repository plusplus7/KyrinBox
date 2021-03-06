#ifndef KYRINBOX_SRC_COMMON_KYRIN_CLUSTER_H_
#define KYRINBOX_SRC_COMMON_KYRIN_CLUSTER_H_

#include <string>
#include <vector>
#include <utility>
#include "configs/kyrin_master_config.h"
#include "configs/kyrin_chunk_config.h"
#include "configs/kyrin_key_center_config.h"

namespace kyrin {
namespace common {

enum KyrinServerType{
    k_type_master = 1,
    k_type_slavenode = 2,
    k_type_chunk = 3,
    k_type_keycenter = 4
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

    uint32_t get_chunk_set_file_key_info_port(uint32_t kbid = 0) {
        if (!kbid)
            kbid = m_kbid;
        return m_chunk_configs[kbid-3001].set_file_key_info_port();
    }

    uint32_t get_chunk_get_file_key_info_port(uint32_t kbid = 0) {
        if (!kbid)
            kbid = m_kbid;
        return m_chunk_configs[kbid-3001].get_file_key_info_port();
    }

    uint32_t get_chunk_upload_chunk_file_port(uint32_t kbid = 0) {
        if (!kbid)
            kbid = m_kbid;
        return m_chunk_configs[kbid-3001].upload_chunk_file_port();
    }

    uint32_t get_chunk_download_chunk_file_port(uint32_t kbid = 0) {
        if (!kbid)
            kbid = m_kbid;
        return m_chunk_configs[kbid-3001].download_chunk_file_port();
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

    std::string& get_keycenter_redis_host() {
        return m_keycenter_config.get_config(kyrin::common::constants::k_json_keycenter_redis_host);
    }

    int get_keycenter_redis_port() {
        return atoi(m_keycenter_config.get_config(kyrin::common::constants::k_json_keycenter_redis_port).c_str());
    }

    const char *get_keycenter_host() {
        return m_keycenter_config.machine_address().c_str();
    }

    uint32_t get_keycenter_get_key_port() {
        return m_keycenter_config.get_key_port();
    }

    uint32_t get_keycenter_set_key_port() {
        return m_keycenter_config.set_key_port();
    }

    uint32_t get_keycenter_get_key_backlog() {
        return m_keycenter_config.get_key_backlog();
    }

    uint32_t get_keycenter_set_key_backlog() {
        return m_keycenter_config.set_key_backlog();
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
    configs::KyrinKeyCenterConfig m_keycenter_config;
};

} /* common */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_KYRIN_CLUSTER_H_ */
