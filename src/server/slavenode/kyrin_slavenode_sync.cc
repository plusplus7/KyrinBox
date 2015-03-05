#include <unistd.h>
#include <pthread.h>
#include "kyrin_slavenode_sync.h"
#include "common/kyrin_log.h"
#include "io/kyrin_http_client.h"
#include "common/configs/kyrin_slavenode_config.h"
#include "common/kyrin_lexicographically_helper.h"
#include "hiredis/hiredis.h"
#include "common/crypto/kyrin_base64.h"
#include "common/kyrin_cluster.h"
#include "protobuf/get_oplog.pb.h"
#include "protobuf/operation_log.pb.h"
#include "protobuf/upload_file.pb.h"

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;
using namespace kyrin::io;

static KyrinLog *logger = KyrinLog::get_instance();
static void *kyrin_slavenode_sync_func(void *arg)
{
    KyrinCluster *cluster = KyrinCluster::get_instance();
    configs::KyrinSlavenodeConfig *config = new configs::KyrinSlavenodeConfig();
    config->read_config_file((char *)"kyrinbox_config.json");
    redisContext *redis_context = redisConnect(config->redis_host().c_str(), config->redis_port());
    delete config;

    if (redis_context != NULL && redis_context->err) {
        logger->log("Redis connect error", redis_context->errstr);
        return NULL;
    }

    string current_id;
    KyrinLexicographicallyHelper::get_zero(current_id);
    srand(time(NULL));
    while (true) {
        usleep(((rand()&15)+5) * 100000);
        logger->log("Slavenode sync", "start sync");

        string response;
        string to_post = crypto::base64_encode((unsigned char const*)current_id.c_str(), current_id.length());
        uint32_t random_master = rand()%(cluster->get_master_count())+1;
        if (!KyrinHttpClient::make_request_post(cluster->get_master_ip(random_master),
                                                cluster->get_master_get_oplog_port(random_master),
                                                "/GetOplog",
                                                response,
                                                to_post)) {
            return NULL;
        }
        kyrinbox::api::GetOplogResponse data;
        data.ParseFromString(crypto::base64_decode(response));
        for (int i=0; i<data.log_data_size(); i++) {
            kyrinbox::api::OperationLog op_log;
            op_log.ParseFromString(data.log_data(i));
            if (op_log.op_type() == 1) {  // Upload file operation
                kyrinbox::api::UploadFileOperation op_data;
                op_data.ParseFromString(op_log.log_data());
                string operation_data;
                if (!op_data.data().SerializeToString(&operation_data)) {
                    logger->log("update_oplog", "Can't serialize op_data to string");
                    break;
                }
                redisReply *redis_reply = (redisReply *)redisCommand(redis_context, "SET %b %b", op_data.key().c_str(), op_data.key().length(), operation_data.c_str(), operation_data.length());
                logger->log("New key", op_data.key().c_str());
                logger->log("New value", operation_data.c_str());
                if (redis_reply != NULL && redis_reply->type == REDIS_REPLY_ERROR) {
                    logger->log("update_oplog", "Can't put in redis");
                    freeReplyObject(redis_reply);
                    break;
                }
                freeReplyObject(redis_reply);
                KyrinLexicographicallyHelper::add_one(current_id);
            }
        }
    }
    return NULL;
}

bool KyrinSlavenodeSync::start_sync()
{
    pthread_t sync_thd;
    if (!pthread_create(&sync_thd, NULL, kyrin_slavenode_sync_func, NULL)) {
        return false;
    }
    return true;
}

} /* server */
} /* kyrin */
