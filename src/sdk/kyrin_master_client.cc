#include "kyrin_master_client.h"
#include "common/crypto/kyrin_base64.h"
#include "common/kyrin_log.h"

namespace kyrin {
namespace sdk {

using namespace kyrin::common;

static KyrinLog *logger = KyrinLog::get_instance();

bool KyrinMasterClient::confirm_oplog_build_request(kyrinbox::api::OperationLogMessage &op_log, RequestData &data)
{
    if (!op_log.SerializeToString(&data.postdata)) {
        logger->log("KyrinMasterClient", "Can't serialize protobuf op_log");
        return false;
    }
    data.postdata = crypto::base64_encode((unsigned char const*)data.postdata.c_str(), data.postdata.length());
    data.timeout = 5;
    data.uri = "/ConfirmOplog";
    return true;
}

} /* sdk */
} /* kyrin */
