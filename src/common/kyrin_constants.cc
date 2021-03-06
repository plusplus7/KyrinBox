#include "kyrin_constants.h"

namespace kyrin {
namespace common {
namespace constants {

const uint32_t k_chunk_size                         = 1024;
const char *k_config_filepath                       = "kyrinbox_config.json";

const char* k_json_server_type                      = "ServerType";
const char* k_json_kbid                             = "Kbid";
const char* k_json_master_server_count              = "MasterServerCount";
const char* k_json_chunk_seed_count                 = "ChunkSeedCount";
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
const char *k_json_slavenode_redis_host             = "SlavenodeRedisHost";
const char *k_json_slavenode_redis_port             = "SlavenodeRedisPort";
const char *k_json_slavenode_download_file_port     = "SlavenodeDownloadFilePort";
const char *k_json_slavenode_download_file_backlog  = "SlavenodeDownloadFileBacklog";

const char* k_json_chunk_gossip_server_port         = "ChunkGossipServerPort";
const char* k_json_chunk_gossip_server_backlog      = "ChunkGossipServerBacklog";
const char* k_json_chunk_machine_address            = "ChunkMachineAddress";
const char* k_json_chunk_role                       = "ChunkRole";
const char* k_json_chunk_upload_chunk_file_port     = "ChunkUploadChunkFilePort";
const char* k_json_chunk_upload_chunk_file_backlog  = "ChunkUploadChunkFileBacklog";
const char* k_json_chunk_download_chunk_file_port   = "ChunkDownloadChunkFilePort";
const char* k_json_chunk_download_chunk_file_backlog= "ChunkDownloadChunkFileBacklog";
const char* k_json_chunk_set_file_key_info_port     = "ChunkSetFileKeyInfoPort";
const char* k_json_chunk_set_file_key_info_backlog  = "ChunkSetFileKeyInfoBacklog";
const char* k_json_chunk_get_file_key_info_port     = "ChunkGetFileKeyInfoPort";
const char* k_json_chunk_get_file_key_info_backlog  = "ChunkGetFileKeyInfoBacklog";

const char *k_json_keycenter_redis_host             = "KeyCenterRedisHost";
const char *k_json_keycenter_redis_port             = "KeyCenterRedisPort";
const char* k_json_keycenter_set_kyrin_key_port     = "KeyCenterSetKyrinKeyPort";
const char* k_json_keycenter_set_kyrin_key_backlog  = "KeyCenterSetKyrinKeyBacklog";
const char* k_json_keycenter_get_kyrin_key_port     = "KeyCenterGetKyrinKeyPort";
const char* k_json_keycenter_get_kyrin_key_backlog  = "KeyCenterGetKyrinKeyBacklog";

const char *k_json_chunk_keyinfo_database_path      = "ChunkKeyinfoDatabasePath";

const uint32_t k_server_max_post_data_size          = 1024*64;
const uint32_t k_server_max_get_oplog_size          = 100;

} /* constants */
} /* common */
} /* kyrin */
