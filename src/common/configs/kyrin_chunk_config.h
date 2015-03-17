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
};

} /* configs */
} /* common */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_KYRIN_CHUNK_CONFIG_H_ */
