#ifndef KYRINBOX_SRC_SERVER_REQUEST_HANDLER_SET_FILE_KEY_INFO_H_
#define KYRINBOX_SRC_SERVER_REQUEST_HANDLER_SET_FILE_KEY_INFO_H_

#include "server/chunk/kyrin_chunk_server.h"
#include "server/chunk/kyrin_chunk_gossiper.h"

namespace kyrin {
namespace server {

class KyrinChunkServer;

class SetFileKeyInfoRequestHandler {
public:
    SetFileKeyInfoRequestHandler(kyrin::server::KyrinChunkGossiper *gossiper,
                                 kyrin::io::KyrinDatabaseWrapper *keyinfo_db)
                                  : m_gossiper(gossiper),
                                    m_keyinfo_db(keyinfo_db)
    {}
    ~SetFileKeyInfoRequestHandler()
    {}

    void handle_request(KyrinChunkServer *server, evhttp_request *req);

private:
    kyrin::server::KyrinChunkGossiper *m_gossiper;
    kyrin::io::KyrinDatabaseWrapper *m_keyinfo_db;
};

} /* server */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_REQUEST_HANDLER_SET_FILE_KEY_INFO_H_ */
