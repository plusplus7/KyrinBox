#ifndef KYRINBOX_SRC_COMMON_KYRIN_CLUSTER_H_
#define KYRINBOX_SRC_COMMON_KYRIN_CLUSTER_H_

#include <string>
#include <vector>
#include "configs/kyrin_master_config.h"

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

    configs::KyrinMasterConfig* get_master_config(uint32_t kbid = 0) {
        if (!kbid)
            kbid = m_kbid;
        return &m_master_configs[kbid-1];
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
    KyrinCluster() {}
    virtual ~KyrinCluster() {}

    static KyrinCluster *m_kyrin_cluster;
    KyrinServerType m_server_type;
    uint32_t m_kbid;
    uint32_t m_master_server_count;
    std::vector<configs::KyrinMasterConfig> m_master_configs;
};

} /* common */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_KYRIN_CLUSTER_H_ */
