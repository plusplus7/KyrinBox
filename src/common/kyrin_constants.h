#ifndef KYRINBOX_SRC_COMMON_KYRIN_CONSTANTS_H_
#define KYRINBOX_SRC_COMMON_KYRIN_CONSTANTS_H_

#include <string>

namespace kyrin {
namespace common {
namespace constants {

extern const char* k_config_filepath;

extern const char* k_json_server_type;
extern const char* k_json_kbid;
extern const char* k_json_master_server_count;
extern const char* k_json_machine_address;
extern const char* k_json_master_upload_file_port;
extern const char* k_json_master_upload_file_backlog;
extern const char* k_json_master_get_oplog_port;
extern const char* k_json_master_get_oplog_backlog;
extern const char* k_json_master_confirm_oplog_port;
extern const char* k_json_master_confirm_oplog_backlog;
extern const char* k_json_master_sentinel_port;

extern const char* k_json_master_server_threads;
extern const char* k_json_master_userdata_database_path;
extern const char* k_json_master_oplog_database_path;

extern const char* k_json_slavenode_redis_host;
extern const char* k_json_slavenode_redis_port;

extern const uint32_t   k_server_max_post_data_size;
extern const uint32_t   k_server_max_get_oplog_size;

} /* constants */
} /* common */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_KYRIN_CONSTANTS_H_ */
