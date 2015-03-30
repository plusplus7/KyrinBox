#include "set_file_key_info_request_handler.h"
#include "common/kyrin_macros.h"
#include "common/crypto/kyrin_base64.h"
#include "protobuf/upload_chunk_file.pb.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

void SetFileKeyInfoRequestHandler::handle_request(KyrinChunkServer *server, evhttp_request *req)
{
    string reply = "Service denied...";
    string request_body = "";

    kyrin_todo();

    if (!server->server_get_postdata(req, request_body)) {
        reply = "Can't read post data...";
        break;
    }
    request_body = crypto::base64_decode(request_body);
    kyrinbox::api::SetFileKeyInfo info;
    info.ParseFromString(request_body);

    string key = info.account()+info.file_name();
    m_keyinfo_db->put(info.account()+info.file_name(), request_body);

    struct stat st = {0};
    char user_dir[1024];
    sprintf(user_dir, "./%s", info.account().c_str());
    if (stat(user_dir, &st) == -1) {
        mkdir(user_dir, 0755);
    }

    sprintf(user_dir, "./%s/%s.chunkfile", info.account().c_str(), info.file_name().c_str());
    FILE *fp = fopen(user_dir, "wb");
    fclose(fp);

    reply = "Set file key info success";
    server->server_send_reply_ok(req, reply);
    return ;

    kyrin_done();

    server->server_send_reply_bad(req, reply);
}

} /* server */
} /* kyrin */
