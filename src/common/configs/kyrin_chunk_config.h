#ifndef KYRINBOX_SRC_COMMON_KYRIN_CHUNK_CONFIG_H_
#define KYRINBOX_SRC_COMMON_KYRIN_CHUNK_CONFIG_H_

#include <map>
#include <cstdlib>
#include "kyrin_base_config.h"
#include "common/kyrin_constants.h"

namespace kyrin {
namespace common {
namespace configs {

class KyrinChunkConfig : public KyrinBaseConfig {
public:
    uint32_t gossip_server_port() {
        return atoi(get_config(kyrin::common::constants::k_json_chunk_gossip_server_port).c_str());
    }

    uint32_t gossip_server_backlog() {
        return atoi(get_config(kyrin::common::constants::k_json_chunk_gossip_server_backlog).c_str());
    }

    std::string& machine_address() {
        return get_config(kyrin::common::constants::k_json_chunk_machine_address);
    }

    std::string& role() {
        return get_config(kyrin::common::constants::k_json_chunk_role);
    }

    uint32_t set_file_key_info_port() {
        return atoi(get_config(kyrin::common::constants::k_json_chunk_set_file_key_info_port).c_str());
    }

    uint32_t set_file_key_info_backlog() {
        return atoi(get_config(kyrin::common::constants::k_json_chunk_set_file_key_info_backlog).c_str());
    }

    uint32_t upload_chunk_file_port() {
        return atoi(get_config(kyrin::common::constants::k_json_chunk_upload_chunk_file_port).c_str());
    }

    uint32_t upload_chunk_file_backlog() {
        return atoi(get_config(kyrin::common::constants::k_json_chunk_upload_chunk_file_backlog).c_str());
    }

    std::string keyinfo_database_path() {
        return get_config(kyrin::common::constants::k_json_chunk_keyinfo_database_path);
    }
};

} /* configs */
} /* common */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_KYRIN_CHUNK_CONFIG_H_ */
