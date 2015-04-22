#include "common/kyrin_macros.h"
#include "common/crypto/kyrin_base64.h"
#include "common/kyrin_log.h"
#include "get_kyrin_key_request_handler.h"
#include "protobuf/kyrin_key.pb.h"
#include <iostream>

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

static KyrinLog *logger = KyrinLog::get_instance();

GetKyrinKeyRequestHandler::GetKyrinKeyRequestHandler(char *host, int port)
{
    m_redis_context = NULL;
    while (m_redis_context == NULL) {
        m_redis_context = redisConnect(host, port);
    }
    redisCommand(m_redis_context, "auth asdf"); /* FIXME: hardcode secret...*/
}

void GetKyrinKeyRequestHandler::handle_request(KyrinKeyCenterServer *server, evhttp_request *req)
{
    string reply = "Error";
    string request_body = "";

    kyrin_todo();

    if (!server->server_get_postdata(req, request_body)) {
        reply = "Can't read post data ...";
        break;
    }
    request_body = crypto::base64_decode(request_body);
    kyrinbox::api::GetKyrinKeyRequest request_pb;
    if (!request_pb.ParseFromString(request_body)) {
        reply = "Bad request...";
        break;
    }
    redisReply *redis_reply = (redisReply *)redisCommand(m_redis_context, "GET %s", request_pb.key_id().c_str());
    if (redis_reply == NULL || redis_reply->type != REDIS_REPLY_STRING) {
        logger->log("get kyrin key", string("Cannot find key: " + request_pb.key_id()).c_str());
        freeReplyObject(redis_reply);
        reply = "No such key";
        break;
    }

    reply = string(redis_reply->str);
    freeReplyObject(redis_reply);
    server->server_send_reply_ok(req, reply);
    return ;

    kyrin_done();

    server->server_send_reply_bad(req, reply);
}

} /* server */
} /* kyrin */
