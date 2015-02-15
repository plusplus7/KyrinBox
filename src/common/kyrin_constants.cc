#include "kyrin_constants.h"

namespace kyrin {
namespace common {
namespace constants {

const char *k_config_filepath                       = "kyrinbox_config.json";

const char* k_json_server_type                      = "ServerType";
const char* k_json_kbid                             = "Kbid";
const char* k_json_master_server_count              = "MasterServerCount";
const char *k_json_machine_address                  = "MachineAddress";
const char *k_json_master_upload_file_port          = "MasterUploadFilePort";
const char *k_json_master_upload_file_backlog       = "MasterUploadFileBacklog";
const char *k_json_master_get_oplog_port            = "MasterGetOplogPort";
const char *k_json_master_get_oplog_backlog         = "MasterGetOplogBacklog";
const char *k_json_master_confirm_oplog_port        = "MasterConfirmOplogPort";
const char *k_json_master_confirm_oplog_backlog     = "MasterConfirmOplogBacklog";
const char *k_json_master_sentinel_port             = "MasterSentinelPort";

const char *k_json_master_server_threads            = "MasterServerThreads";
const char *k_json_master_userdata_database_path    = "MasterUserdataDatabasePath";
const char *k_json_master_oplog_database_path       = "MasterOplogDatabasePath";
const uint32_t k_server_max_post_data_size          = 1024;
const uint32_t k_server_max_get_oplog_size          = 100;

} /* constants */
} /* common */
} /* kyrin */
