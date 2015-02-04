#ifndef KYRINBOX_SRC_COMMON_KYRIN_CONFIG_H_
#define KYRINBOX_SRC_COMMON_KYRIN_CONFIG_H_
#include <string>
#include <map>

namespace kyrin {
namespace common {

class KyrinConfig {
public:
    static KyrinConfig *get_instance();

    bool read_config_file(const char *filename);
    std::string &get_config(const char *key);
private:
    KyrinConfig();

    static KyrinConfig *m_kyrin_config;
    std::map<std::string, std::string> config_map;
};

} /* common */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_KYRIN_CONFIG_H_ */
