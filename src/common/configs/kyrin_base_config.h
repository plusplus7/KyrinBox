#ifndef KYRINBOX_SRC_COMMON_CONFIGS_KYRIN_BASE_CONFIG_H_
#define KYRINBOX_SRC_COMMON_CONFIGS_KYRIN_BASE_CONFIG_H_

#include <map>

namespace kyrin {
namespace common {
namespace configs {

class KyrinBaseConfig {
public:
    bool read_config_file(char *filename);
    std::string &get_config(const char *key);

private:
    std::map<std::string, std::string> m_config_map;
};

} /* configs */
} /* common */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_CONFIGS_KYRIN_BASE_CONFIG_H_ */
