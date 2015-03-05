#include "download_file_request_handler.h"
#include "common/kyrin_macros.h"
#include "common/kyrin_log.h"
#include "protobuf/upload_file.pb.h"
#include <sstream>

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

static KyrinLog *logger = KyrinLog::get_instance();
DownloadFileRequestHandler::DownloadFileRequestHandler(char *host, int port)
{
    m_redis_context = redisConnect(host, port);
}

void DownloadFileRequestHandler::handle_request(KyrinBaseServer *server, evhttp_request *req)
{
    string reply = "";
    string request_body = "";
    kyrin_todo();

    if (!server->server_get_postdata(req, request_body)) {
        reply = "Can't read post";
        break;
    }
    logger->log("DownloadFileRequestHandler handle", request_body.c_str());
    reply = "download it!";

    redisReply *redis_reply = (redisReply *)redisCommand(m_redis_context, "GET %b", request_body.c_str(), request_body.length());
    if (redis_reply == NULL || redis_reply->type != REDIS_REPLY_STRING) {
        logger->log("DownloadFileRequestHandler handle request failed body", request_body.c_str());
        reply = "No such key";
        break;
    }
    kyrinbox::api::UploadFileResponse response;
    response.ParseFromString(redis_reply->str);
    stringstream ss;
    ss.clear();
    for (uint32_t i=0; i<response.file_hosts_size(); i++) {
        ss << response.file_hosts(i) << " | ";
    }
    ss << response.file_size() << " | ";
    ss << response.merkle_sha1() << "\n";
    reply = ss.str();
    server->server_send_reply_ok(req, reply);
    freeReplyObject(redis_reply);
    return ;

    kyrin_done();
    server->server_send_reply_bad(req, reply);
    return ;
}

} /* server */
} /* kyrin */
