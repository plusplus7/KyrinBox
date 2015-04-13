#ifndef KYRINBOX_SRC_SERVER_REQUEST_HANDLER_SET_KYRIN_KEY_REQUEST_HANDLER_H_
#define KYRINBOX_SRC_SERVER_REQUEST_HANDLER_SET_KYRIN_KEY_REQUEST_HANDLER_H_

#include "server/keycenter/kyrin_key_center_server.h"
#include "common/kyrin_rsa_helper.h"
#include "hiredis/hiredis.h"

namespace kyrin {
namespace server {

class KyrinKeyCenterServer;

class SetKyrinKeyRequestHandler {
public:
    SetKyrinKeyRequestHandler(char *host, int port);
    ~SetKyrinKeyRequestHandler()
    {}
    void handle_request(KyrinKeyCenterServer *server, evhttp_request *req);

private:
    redisContext *m_redis_context;
};

} /* server */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_REQUEST_HANDLER_SET_KYRIN_KEY_REQUEST_HANDLER_H_ */
