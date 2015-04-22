#ifndef KYRINBOX_SRC_SDK_KYRIN_MASTER_CLIENT_H_
#define KYRINBOX_SRC_SDK_KYRIN_MASTER_CLIENT_H_

#include "kyrin_client.h"
#include "protobuf/operation_log.pb.h"

namespace kyrin {
namespace sdk {

class KyrinMasterClient : public KyrinClient {
public:
    bool confirm_oplog_build_request(std::string &op_id, int state, , RequestData &data);
    bool confirm_oplog_parse_response()
};

} /* sdk */
} /* kyrin */
#endif /* KYRINBOX_SRC_SDK_KYRIN_MASTER_CLIENT_H_ */
