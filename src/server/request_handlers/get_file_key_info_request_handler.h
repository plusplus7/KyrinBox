#ifndef KYRINBOX_SRC_SERVER_REQUEST_HANDLER_GET_FILE_KEY_INFO_REQUEST_HANDLER_H_
#define KYRINBOX_SRC_SERVER_REQUEST_HANDLER_GET_FILE_KEY_INFO_REQUEST_HANDLER_H_

#include "server/chunk/kyrin_chunk_server.h"

namespace kyrin {
namespace server {

class KyrinChunkServer;

class GetFileKeyInfoRequestHandler {
public:
    GetFileKeyInfoRequestHandler(kyrin::io::KyrinDatabaseWrapper *keyinfo_db)
                                : m_keyinfo_db(keyinfo_db)
    {}
    ~GetFileKeyInfoRequestHandler()
    {}
    void handle_request(KyrinChunkServer *server, evhttp_request *req);

private:
    kyrin::io::KyrinDatabaseWrapper *m_keyinfo_db;
};

} /* server */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_REQUEST_HANDLER_GET_FILE_KEY_INFO_REQUEST_HANDLER_H_ */
