#ifndef KYRINBOX_SRC_SERVER_REQUEST_HANDLER_GET_OPLOG_REQUEST_HANDLER_H_
#define KYRINBOX_SRC_SERVER_REQUEST_HANDLER_GET_OPLOG_REQUEST_HANDLER_H_

#include "io/kyrin_database_wrapper.h"
#include "server/master/kyrin_master_server.h"
#include "server/master/kyrin_master_sentinel.h"

namespace kyrin {
namespace server {

class KyrinMasterServer;

class GetOplogRequestHandler {
public:
    GetOplogRequestHandler(kyrin::io::KyrinDatabaseWrapper *oplog_db)
                            : m_oplog_db(oplog_db)
    {}

    ~GetOplogRequestHandler()
    {}

    void handle_request(KyrinMasterServer *server, evhttp_request *req);

private:
    kyrin::io::KyrinDatabaseWrapper *m_oplog_db;
};

} /* server */
} /* kyrin */
#endif /* end of include guard: KYRINBOX_SRC_SERVER_REQUEST_HANDLER_GET_OPLOG_REQUEST_HANDLER_H_ */
