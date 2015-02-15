#ifndef KYRINBOX_SRC_COMMON_KYRIN_MASTER_CONFIG_H_
#define KYRINBOX_SRC_COMMON_KYRIN_MASTER_CONFIG_H_

#include <map>
#include <cstdlib>
#include "kyrin_base_config.h"
#include "common/kyrin_constants.h"

namespace kyrin {
namespace common {
namespace configs {

class KyrinMasterConfig : public KyrinBaseConfig {
public:
    uint32_t upload_file_port() {
        return atoi(get_config(kyrin::common::constants::k_json_master_upload_file_port).c_str());
    }

    uint32_t confirm_oplog_port() {
        return atoi(get_config(kyrin::common::constants::k_json_master_confirm_oplog_port).c_str());
    }
    
    uint32_t get_oplog_port() {
        return atoi(get_config(kyrin::common::constants::k_json_master_get_oplog_port).c_str());
    }

    uint32_t sentinel_port() {
        return atoi(get_config(kyrin::common::constants::k_json_master_sentinel_port).c_str());
    }

    uint32_t upload_file_backlog() {
        return atoi(get_config(kyrin::common::constants::k_json_master_upload_file_backlog).c_str());
    }

    uint32_t confirm_oplog_backlog() {
        return atoi(get_config(kyrin::common::constants::k_json_master_confirm_oplog_backlog).c_str());
    }

    uint32_t get_oplog_backlog() {
        return atoi(get_config(kyrin::common::constants::k_json_master_get_oplog_backlog).c_str());
    }

    uint32_t server_threads() {
        return atoi(get_config(kyrin::common::constants::k_json_master_server_threads).c_str());
    }

    std::string& userdata_database_path() {
        return get_config(kyrin::common::constants::k_json_master_userdata_database_path);
    }

    std::string& oplog_database_path() {
        return get_config(kyrin::common::constants::k_json_master_oplog_database_path);
    }

    std::string& machine_address() {
        return get_config(kyrin::common::constants::k_json_machine_address);
    }
};

} /* configs */
} /* common */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_KYRIN_MASTER_CONFIG_H_ */
