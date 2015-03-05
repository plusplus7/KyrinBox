#ifndef KYRINBOX_SRC_SERVER_REQUEST_HANDLER_DOWNLOAD_FILE_REQUEST_HANDLER_H_
#define KYRINBOX_SRC_SERVER_REQUEST_HANDLER_DOWNLOAD_FILE_REQUEST_HANDLER_H_

#include "server/slavenode/kyrin_slavenode_server.h"

namespace kyrin {
namespace server {

class DownloadFileRequestHandler {
public:
    DownloadFileRequestHandler()
    {}
    ~DownloadFileRequestHandler()
    {}
    void handle_request(KyrinBaseServer *server, evhttp_request *req);

private:

};

} /* server */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_REQUEST_HANDLER_DOWNLOAD_FILE_REQUEST_HANDLER_H_ */
