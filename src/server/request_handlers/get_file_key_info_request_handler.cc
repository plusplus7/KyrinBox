#include "get_file_key_info_request_handler.h"
#include "common/kyrin_macros.h"
#include "protobuf/upload_chunk_file.pb.h"
#include "common/crypto/kyrin_base64.h"

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

void GetFileKeyInfoRequestHandler::handle_request(KyrinChunkServer *server, evhttp_request *req)
{
    string reply = "Service denied...";
    string request_body = "";

    kyrin_todo();

    if (!server->server_get_postdata(req, request_body)) {
        reply = "Can't read post data...";
        break;
    }
    request_body = crypto::base64_decode(request_body);
    kyrinbox::api::GetFileKeyInfo info;
    info.ParseFromString(request_body);

    string key = info.account()+info.file_name();
    if (!m_keyinfo_db->get(key, reply)) {
        reply = "No such file";
        break;
    }
    reply = crypto::base64_encode((unsigned char const*)reply.c_str(), reply.length());
    server->server_send_reply_ok(req, reply);
    return ;

    kyrin_done();

    server->server_send_reply_bad(req, reply);
}

} /* server */
} /* kyrin */
