#include "kyrin_base_config.h"
#include <fstream>

namespace kyrin {
namespace common {
namespace configs {

using namespace std;

bool KyrinBaseConfig::read_config_file(char *filename)
{
    ifstream file(filename);
    string key = "";
    string value = "";

    m_config_map.clear();
    file >> key >> key;
    while (file >> key >> value >> value)
        m_config_map[key.substr(1, key.length()-2)] = value.substr(1, value.length()-(value.back()==','?3:2));

    file.close();
    return true;
}

string &KyrinBaseConfig::get_config(const char *key)
{
    if (m_config_map.count(key)) {
        return m_config_map[key];
    } else {
        return m_config_map.begin()->second;
    }
}

} /* configs */
} /* common */
} /* kyrin */
