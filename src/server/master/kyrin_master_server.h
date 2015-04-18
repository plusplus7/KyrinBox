#ifndef KYRINBOX_SRC_SERVER_KYRIN_MASTER_SERVER_H_
#define KYRINBOX_SRC_SERVER_KYRIN_MASTER_SERVER_H_

#include "server/base/kyrin_base_server.h"
#include "server/request_handlers/upload_file_request_handler.h"
#include "server/request_handlers/get_oplog_request_handler.h"
#include "server/request_handlers/confirm_oplog_request_handler.h"
#include "server/request_filters/examine_identity_request_filter.h"
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

    void process_upload_file_request(evhttp_request *req);
    void process_get_oplog_request(evhttp_request *req);
    void process_confirm_oplog_request(evhttp_request *req);

private:
    int upload_file_fd;
    int get_oplog_fd;
    int confirm_oplog_fd;
    kyrin::server::KyrinMasterSentinel *m_sentinel;
    kyrin::io::KyrinDatabaseWrapper *m_userdata_db;
    kyrin::io::KyrinDatabaseWrapper *m_oplog_db;
    UploadFileRequestHandler *m_upload_file_request_handler;
    GetOplogRequestHandler *m_get_oplog_request_handler;
    ConfirmOplogRequestHandler *m_confirm_oplog_request_handler;

    ExamineIdentityRequestFilter *m_examine_identity_request_filter;

};

} /* server */
} /* kyrin */

#endif /* end of include guard KYRINBOX_SRC_SERVER_KYRIN_MASTER_SERVER_H_ */
