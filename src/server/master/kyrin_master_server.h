#ifndef KYRINBOX_SRC_SERVER_KYRIN_MASTER_SERVER_H_
#define KYRINBOX_SRC_SERVER_KYRIN_MASTER_SERVER_H_

#include "server/base/kyrin_base_server.h"
#include "server/request_handlers/upload_file_request_handler.h"
#include "server/request_handlers/get_oplog_request_handler.h"
#include "kyrin_master_sentinel.h"
#include "io/kyrin_database_wrapper.h"

namespace kyrin {
namespace server {

class UploadFileRequestHandler;

class KyrinMasterServer : public KyrinBaseServer {
public:
    bool server_initialize(kyrin::server::KyrinMasterSentinel *sentinel);
    bool server_start();
    bool server_free();
    bool server_set_processor(evhttp *server, int thread_code);

    UploadFileRequestHandler* get_upload_file_request_handler() {
        return upload_file_request_handler;
    }

    GetOplogRequestHandler* get_get_oplog_request_handler() {
        return get_oplog_request_handler;
    }

private:
    int upload_file_fd;
    int get_oplog_fd;
    int get_confirm_fd;
    kyrin::server::KyrinMasterSentinel *m_sentinel;
    kyrin::io::KyrinDatabaseWrapper *m_userdata_db;
    kyrin::io::KyrinDatabaseWrapper *m_oplog_db;
    UploadFileRequestHandler *upload_file_request_handler;
    GetOplogRequestHandler *get_oplog_request_handler;

};

} /* server */

} /* kyrin */

#endif /* end of include guard KYRINBOX_SRC_SERVER_KYRIN_MASTER_SERVER_H_ */
