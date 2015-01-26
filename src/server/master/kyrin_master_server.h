#ifndef KYRINBOX_SRC_SERVER_KYRIN_MASTER_SERVER_H_
#define KYRINBOX_SRC_SERVER_KYRIN_MASTER_SERVER_H_

#include "server/base/kyrin_base_server.h"
#include "io/kyrin_database_wrapper.h"

namespace kyrin {
namespace server {

class KyrinMasterServer : public KyrinBaseServer {
public:
    bool server_initialize();
    bool server_start();
    bool server_free();
    bool server_set_processor(evhttp *server, int thread_code);
    kyrin::io::KyrinDatabaseWrapper* get_userdata_db()
    {
        return m_userdata_db;
    }
    kyrin::io::KyrinDatabaseWrapper* get_oplog_db()
    {
        return m_oplog_db;
    }

private:
    int upload_file_fd;
    kyrin::io::KyrinDatabaseWrapper *m_userdata_db;
    kyrin::io::KyrinDatabaseWrapper *m_oplog_db;

};

} /* server */

} /* kyrin */

#endif /* end of include guard KYRINBOX_SRC_SERVER_KYRIN_MASTER_SERVER_H_ */
