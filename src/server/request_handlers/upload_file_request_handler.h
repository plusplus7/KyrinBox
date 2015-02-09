#ifndef KYRINBOX_SRC_SERVER_REQUEST_HANDLER_UPLOAD_FILE_REQUEST_HANDLER_H_
#define KYRINBOX_SRC_SERVER_REQUEST_HANDLER_UPLOAD_FILE_REQUEST_HANDLER_H_

#include <vector>
#include "io/kyrin_database_wrapper.h"
#include "server/master/kyrin_master_server.h"
#include "server/master/kyrin_master_sentinel.h"

namespace kyrin {
namespace server {

class KyrinMasterServer;

class UploadFileRequestHandler {
public:
    UploadFileRequestHandler(KyrinMasterSentinel *sentinel,
                    kyrin::io::KyrinDatabaseWrapper *user_db,
                    kyrin::io::KyrinDatabaseWrapper *oplog_db)
                    : m_sentinel(sentinel),
                      m_userdata_db(user_db),
                      m_oplog_db(oplog_db)
    {
        m_masters = sentinel->get_all_masters();
        m_kbid = sentinel->get_kbid();
    }

    ~UploadFileRequestHandler()
    {}

    void handle_request(KyrinMasterServer *server, evhttp_request *req);

private:
    kyrin::server::KyrinMasterSentinel *m_sentinel;
    kyrin::io::KyrinDatabaseWrapper *m_userdata_db;
    kyrin::io::KyrinDatabaseWrapper *m_oplog_db;
    std::vector<KyrinMachineInfo> m_masters;
    uint64_t m_kbid;
};

} /* server */
} /* kyrin */
#endif /* end of include guard: KYRINBOX_SRC_SERVER_REQUEST_HANDLER_UPLOAD_FILE_REQUEST_HANDLER_H_ */
