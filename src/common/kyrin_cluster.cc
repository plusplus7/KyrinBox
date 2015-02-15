#include "kyrin_cluster.h"
#include "kyrin_constants.h"
#include <fstream>
#include <iostream>
#include <string>

namespace kyrin {
namespace common {

using namespace std;
using namespace kyrin::common::configs;

KyrinCluster *KyrinCluster::m_kyrin_cluster = NULL;

KyrinCluster *KyrinCluster::get_instance()
{
    if (m_kyrin_cluster == NULL) {
        m_kyrin_cluster = new KyrinCluster();
        m_kyrin_cluster->read_config_file(constants::k_config_filepath);
    }
    return m_kyrin_cluster;
}

bool KyrinCluster::read_config_file(const char *filename)
{
    ifstream file(filename);
    map<string, string> t_map;
    t_map.clear();
    string key = "";
    string value = "";

    file>>key>>key; // Ignore the first line and '{'

    while (file >> key >> value >> value)
        t_map[key.substr(1, key.length()-2)] = value.substr(1, value.length()-(value.back()==','?3:2));
    file.close();
    if (t_map.count(constants::k_json_server_type) == 0
     || t_map.count(constants::k_json_kbid) == 0
     || t_map.count(constants::k_json_master_server_count) == 0)
        return false;

    if (t_map[constants::k_json_server_type] == "Master") {
        m_server_type = k_type_master;
    } else if (t_map[constants::k_json_server_type] == "Slavenode") {
        m_server_type = k_type_slavenode;
    } else {
        m_server_type = k_type_chunk;
    }

    m_kbid = atoi(t_map[constants::k_json_kbid].c_str());
    m_master_server_count = atoi(t_map[constants::k_json_master_server_count].c_str());

    m_master_configs.clear();
    for (uint32_t i = 1; i <= m_master_server_count; i++) {
        KyrinMasterConfig master_config;
        char t_filename[40];
        sprintf(t_filename, "kyrinbox_master_config_%u.json", i);
        master_config.read_config_file(t_filename);
        m_master_configs.push_back(master_config);
    }
    return true;
}

} /* common */
} /* kyrin */
