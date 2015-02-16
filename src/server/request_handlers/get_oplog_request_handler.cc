#include "common/kyrin_log.h"
#include "common/kyrin_constants.h"
#include "common/kyrin_macros.h"
#include "common/kyrin_lexicographically_helper.h"
#include "common/crypto/kyrin_base64.h"
#include "upload_file_request_handler.h"
#include "protobuf/get_oplog.pb.h"

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

static KyrinLog *logger = KyrinLog::get_instance();
void GetOplogRequestHandler::handle_request(KyrinMasterServer *server, evhttp_request *req)
{
    string reply = "";
    string request_body = "";
    if (!server->server_get_postdata(req, request_body)) {
        reply = "Can't read post";
        server->server_send_reply_bad(req, reply);
        return ;
    }
    logger->log("handle", request_body.c_str());
    request_body = crypto::base64_decode(request_body);

    if (!m_oplog_db->exist(request_body)) {
        reply = "No such op id";
        server->server_send_reply_bad(req, reply);
        return ;
    }

    leveldb::Iterator* it = m_oplog_db->new_iterator();
    kyrinbox::api::GetOplogResponse response;
    int cnt = 0;
    it->Seek(request_body);
    if (!it->Valid()) {
        reply = "Invalid status";
        server->server_send_reply_bad(req, reply);
        return ;
    }
    while (true) {
        string key = it->key().ToString();
        *(response.add_log_data()) = it->value().ToString();
        cnt++;
        it->Next();
        if (!(it->Valid()) || cnt == constants::k_server_max_get_oplog_size) {
            response.set_last_id(key);
            break;
        }
    }
    delete it;

    response.SerializeToString(&reply);
    reply = crypto::base64_encode((unsigned char const*)reply.c_str(), reply.length());
    server->server_send_reply_ok(req, reply);
}

} /* server */
} /* kyrin */
