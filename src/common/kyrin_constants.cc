#include "kyrin_constants.h"

namespace kyrin {
namespace common {
namespace constants {

const char *k_config_filepath                = "kyrinbox_master_config.json";
const char *k_json_master_reader_port        = "MasterReaderPort";
const char *k_json_master_processor_port     = "MasterProcessorPort";
const char *k_json_master_reader_backlog     = "MasterReaderBacklog";
const char *k_json_master_processor_backlog  = "MasterProcessorBacklog";
const char *k_json_master_server_threads     = "MasterServerThreads";
const char *k_json_master_database_path      = "MasterDatabasePath";
const int   k_server_max_post_data_size      = 1024;

} /* constants */
} /* common */
} /* kyrin */
