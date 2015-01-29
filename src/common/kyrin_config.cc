#include "kyrin_config.h"
#include "kyrin_constants.h"
#include <fstream>
#include <iostream>
#include <string>

namespace kyrin {
namespace common {

using namespace std;

KyrinConfig *KyrinConfig::m_kyrin_config = NULL;

KyrinConfig *KyrinConfig::get_instance()
{
    if (m_kyrin_config == NULL) {
        m_kyrin_config = new KyrinConfig();
        m_kyrin_config->read_config_file(constants::k_config_filepath);
    }
    return m_kyrin_config;
}

KyrinConfig::KyrinConfig()
{
    config_map.clear();

    config_map[constants::k_json_master_upload_file_port]              = "7770";
    config_map[constants::k_json_master_upload_file_backlog]           = "8192";
    config_map[constants::k_json_master_get_oplog_port]                = "7070";
    config_map[constants::k_json_master_get_oplog_backlog]             = "8192";
    config_map[constants::k_json_master_confirm_oplog_port]            = "7700";
    config_map[constants::k_json_master_confirm_oplog_backlog]         = "8192";

    config_map[constants::k_json_master_server_threads]             = "4";
    config_map[constants::k_json_master_userdata_database_path]     = "/tmp/kyrinmaster_userdata_db";
    config_map[constants::k_json_master_oplog_database_path]        = "/tmp/kyrinmaster_oplog_db";
}

bool KyrinConfig::read_config_file(const char *filename)
{
    ifstream file(filename);
    string key = "";
    string value = "";

    file>>key>>key; // Ignore the first line and '{'

    while (file >> key >> value >> value)
        config_map[key.substr(1, key.length()-2)] = value.substr(1, value.length()-(value.back()==','?3:2));

    file.close();
    return true;
}

string &KyrinConfig::get_config(const char *key)
{
    if (config_map.find(key) != config_map.end()) {
        return config_map[key];
    } else {
        return config_map.begin()->second;
    }
}

} /* common */
} /* kyrin */
