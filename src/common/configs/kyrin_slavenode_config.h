#ifndef KYRINBOX_SRC_COMMON_KYRIN_SLAVENODE_CONFIG_H_
#define KYRINBOX_SRC_COMMON_KYRIN_SLAVENODE_CONFIG_H_

#include <map>
#include <cstdlib>
#include "kyrin_base_config.h"
#include "common/kyrin_constants.h"

namespace kyrin {
namespace common {
namespace configs {

class KyrinSlavenodeConfig : public KyrinBaseConfig {
public:
    std::string& redis_host() {
        return get_config(kyrin::common::constants::k_json_slavenode_redis_host);
    }

    int redis_port() {
        return atoi(get_config(kyrin::common::constants::k_json_slavenode_redis_port).c_str());
    }
};

} /* configs */
} /* common */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_KYRIN_SLAVENODE_CONFIG_H_ */
