#include "common/kyrin_macros.h"
#include "common/kyrin_log.h"
#include "set_kyrin_key_request_handler.h"

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

static KyrinLog *logger = KyrinLog::get_instance();

SetKyrinKeyRequestHandler::SetKyrinKeyRequestHandler(char *host, int port)
{
    m_redis_context = redisConnect(host, port);
}

void SetKyrinKeyRequestHandler::handle_request(KyrinKeyCenterServer *server, evhttp_request *req)
{
    string reply = "ok";
    server->server_send_reply_ok(req, reply);
}

} /* server */
} /* kyrin */
