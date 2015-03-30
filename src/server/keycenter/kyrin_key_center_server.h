#ifndef KYRINBOX_SRC_SERVER_KEY_CENTER_KYRIN_KEY_CENTER_SERVER_H_
#define KYRINBOX_SRC_SERVER_KEY_CENTER_KYRIN_KEY_CENTER_SERVER_H_

#include "server/base/kyrin_base_server.h"
#include "server/request_handlers/set_kyrin_key_request_handler.h"
#include "server/request_handlers/get_kyrin_key_request_handler.h"

namespace kyrin {
namespace server {

class SetKyrinKeyRequestHandler;
class GetKyrinKeyRequestHandler;

class KyrinKeyCenterServer : public KyrinBaseServer {
public:
    bool server_initialize();
    bool server_start();
    bool server_free();
    bool server_set_processor(evhttp *server, int thread_code);

    SetKyrinKeyRequestHandler *get_set_kyrin_key_request_handler() {
        return set_kyrin_key_request_handler;
    }

    GetKyrinKeyRequestHandler *get_get_kyrin_key_request_handler() {
        return get_kyrin_key_request_handler;
    }

private:
    int set_key_fd;
    int get_key_fd;
    SetKyrinKeyRequestHandler *set_kyrin_key_request_handler;
    GetKyrinKeyRequestHandler *get_kyrin_key_request_handler;
};

} /* server */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_KEY_CENTER_KYRIN_KEY_CENTER_SERVER_H_ */
