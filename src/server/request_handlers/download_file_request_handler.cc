#include "download_file_request_handler.h"
#include "common/kyrin_macros.h"
#include "common/kyrin_log.h"

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

static KyrinLog *logger = KyrinLog::get_instance();
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
    server->server_send_reply_ok(req, reply);
    return ;

    kyrin_done();
    server->server_send_reply_bad(req, reply);
    return ;
}

} /* server */
} /* kyrin */
