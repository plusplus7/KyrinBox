#ifndef KYRINBOX_SRC_SERVER_REQUEST_HANDLER_CONFIRM_OPLOG_REQUEST_HANDLER_H_
#define KYRINBOX_SRC_SERVER_REQUEST_HANDLER_CONFIRM_OPLOG_REQUEST_HANDLER_H_

#include "io/kyrin_database_wrapper.h"
#include "server/master/kyrin_master_server.h"
#include "server/master/kyrin_master_sentinel.h"
#include "protobuf/operation_log.pb.h"
#include "protobuf/upload_file.pb.h"

namespace kyrin {
namespace server {

class KyrinMasterServer;

class ConfirmOplogRequestHandler {
public:
    ConfirmOplogRequestHandler(KyrinMasterSentinel *sentinel,
                               kyrin::io::KyrinDatabaseWrapper *user_db,
                               kyrin::io::KyrinDatabaseWrapper *oplog_db)
                             : m_oplog_db(oplog_db),
                               m_userdata_db(user_db),
                               m_sentinel(sentinel),
                               m_state(false)
    {}

    ~ConfirmOplogRequestHandler()
    {}

    void handle_request(KyrinMasterServer *server, evhttp_request *req);

private:
    bool update_oplog(std::string &last_id, std::string &target_id);

    kyrin::io::KyrinDatabaseWrapper *m_oplog_db;
    kyrin::io::KyrinDatabaseWrapper *m_userdata_db;
    KyrinMasterSentinel *m_sentinel;
    bool m_state;
    std::string m_state_id;
    kyrinbox::api::OperationLog m_state_data;
};

} /* server */
} /* kyrin */
#endif /* end of include guard: KYRINBOX_SRC_SERVER_REQUEST_HANDLER_CONFIRM_OPLOG_REQUEST_HANDLER_H_ */
