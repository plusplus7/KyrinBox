#ifndef KYRINBOX_SRC_COMMON_KYRIN_KEY_CENTER_CONFIG_H_
#define KYRINBOX_SRC_COMMON_KYRIN_KEY_CENTER_CONFIG_H_

#include <map>
#include <cstdlib>
#include "kyrin_base_config.h"
#include "common/kyrin_constants.h"

namespace kyrin {
namespace common {
namespace configs {

class KyrinKeyCenterConfig : public KyrinBaseConfig {
public:
    std::string& redis_host() {
        return get_config(kyrin::common::constants::k_json_keycenter_redis_host);
    }

    int redis_port() {
        return atoi(get_config(kyrin::common::constants::k_json_keycenter_redis_port).c_str());
    }

    int get_key_port() {
        return atoi(get_config(kyrin::common::constants::k_json_keycenter_get_kyrin_key_port).c_str());
    }

    int get_key_backlog() {
        return atoi(get_config(kyrin::common::constants::k_json_keycenter_get_kyrin_key_backlog).c_str());
    }

    int set_key_port() {
        return atoi(get_config(kyrin::common::constants::k_json_keycenter_set_kyrin_key_port).c_str());
    }

    int set_key_backlog() {
        return atoi(get_config(kyrin::common::constants::k_json_keycenter_set_kyrin_key_backlog).c_str());
    }
};

} /* configs */
} /* common */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_KYRIN__KEY_CENTER_CONFIG_H_ */
