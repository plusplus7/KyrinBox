#include "examine_identity_request_filter.h"

namespace kyrin {
namespace server {

bool ExamineIdentityRequestFilter::filter_request(KyrinBaseServer *server, evhttp_request *req, string &reply, string &digest, string &issuer)
{
    string digest    = "";
    string signature = "";
    if (!server->server_get_header(req, "KYRIN-ISSUER", issuer)) {
        reply = "Can't read post KYRIN-ISSUER..";
        return false;
    }
    if (!server->server_get_digest(req, reply, digest)) {
        reply = "Can't read digest"; 
        return false;
    }
    return m_rsa_helper.examine_legality_from_user(issuer, digest, signature);
}   

} /* server */
} /* kyrin */
