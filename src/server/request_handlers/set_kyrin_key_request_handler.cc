#include "common/kyrin_macros.h"
#include "common/crypto/kyrin_base64.h"
#include "common/kyrin_macros.h"
#include "common/kyrin_log.h"
#include "set_kyrin_key_request_handler.h"
#include "protobuf/kyrin_key.pb.h"
#include <iostream>

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

static KyrinLog *logger = KyrinLog::get_instance();

SetKyrinKeyRequestHandler::SetKyrinKeyRequestHandler(char *host, int port)
{
    m_redis_context = NULL;
    while (m_redis_context == NULL) {
        m_redis_context = redisConnect(host, port);
    }
    redisCommand(m_redis_context, "auth asdf"); /* FIXME: hardcode secret...*/
}

void SetKyrinKeyRequestHandler::handle_request(KyrinKeyCenterServer *server, evhttp_request *req)
{
    string reply = "error";
    string request_body;

    kyrin_todo();

    if (!server->server_get_postdata(req, request_body)) {
        reply = "Can't read post data ...";
        break;
    }
    request_body = crypto::base64_decode(request_body);
    kyrinbox::api::SetKyrinKeyRequest request_pb;
    request_pb.ParseFromString(request_body);

    redisReply *redis_reply = (redisReply *)redisCommand(m_redis_context, "GET KeyCenterAdminPublicKey");
    if (redis_reply == NULL || redis_reply->type != REDIS_REPLY_STRING) {
        logger->log("set kyrin key", "Can not find KeyCenterAdminPublicKey");
        reply = "Failed to authenticate";
        break;
    }
    string public_key(redis_reply->str);
    string signature = "";
    string digest = "";
    if (!server->server_get_header(req, "KYRIN-SIGNATURE", signature)) {
        reply = "No signature found";
        break;
    }
    signature = crypto::base64_decode(signature);
    server->server_get_digest(req, reply, digest);
    if (!KyrinRsaHelper::examine_legality(public_key, digest, signature)) {
        reply = "Unauthorized";
        break;
    }
    freeReplyObject(redis_reply);
    redis_reply = (redisReply *)redisCommand(m_redis_context, "SET %s %s", request_pb.key_id().c_str(), request_pb.key_public().c_str());

    if (redis_reply != NULL && redis_reply->type == REDIS_REPLY_ERROR) {
        freeReplyObject(redis_reply);
        logger->log("set kyrin key", (string("Cannot set kyrin key") + request_pb.key_id()).c_str());
        reply = "Failed to write key";
        break;
    }

    reply = "ok";
    freeReplyObject(redis_reply);
    server->server_send_reply_ok(req, reply);
    return ;

    kyrin_done();

    server->server_send_reply_bad(req, reply);
}

} /* server */
} /* kyrin */
