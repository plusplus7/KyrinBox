#ifndef KYRINBOX_SRC_SERVER_REQUEST_HANDLER_GET_KYRIN_KEY_REQUEST_HANDLER_H_
#define KYRINBOX_SRC_SERVER_REQUEST_HANDLER_GET_KYRIN_KEY_REQUEST_HANDLER_H_

#include "server/keycenter/kyrin_key_center_server.h"
#include "hiredis/hiredis.h"

namespace kyrin {
namespace server {

class KyrinKeyCenterServer;

class GetKyrinKeyRequestHandler {
public:
    GetKyrinKeyRequestHandler(char *host, int port);
    ~GetKyrinKeyRequestHandler()
    {}
    void handle_request(KyrinKeyCenterServer *server, evhttp_request *req);

private:
    redisContext *m_redis_context;
};

} /* server */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_REQUEST_HANDLER_GET_KYRIN_KEY_REQUEST_HANDLER_H_ */
