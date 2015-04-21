#include "examine_identity_request_filter.h"
#include "common/crypto/kyrin_base64.h"
#include <iostream>

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

bool ExamineIdentityRequestFilter::filter_request(KyrinBaseServer *server, evhttp_request *req)
{
    string reply     = "";
    string digest    = "";
    string issuer    = "";
    string signature = "";
    if (!server->server_get_header(req, "KYRIN-SIGNATURE", signature)) {
        reply = "Can't read KYRIN-SIGNATURE..";
        server->server_send_reply_bad(req, reply);
        return false;
    }
    signature = crypto::base64_decode(signature);
    if (!server->server_get_header(req, "KYRIN-ISSUER", issuer)) {
        reply = "Can't read KYRIN-ISSUER..";
        server->server_send_reply_bad(req, reply);
        return false;
    }
    if (!server->server_get_digest(req, reply, digest)) {
        reply = "Can't read digest"; 
        server->server_send_reply_bad(req, reply);
        return false;
    }
    if (!m_rsa_helper->examine_legality_from_user(issuer, digest, signature)) {
        reply = "Bad signature"; 
        server->server_send_reply_bad(req, reply);
        return false;
    }
    return true;
}   

} /* server */
} /* kyrin */
