#include "kyrin_rsa_helper.h"
#include "kyrin_log.h"
#include "common/crypto/kyrin_rsa.h"
#include "io/kyrin_http_client.h"
#include "common/crypto/kyrin_base64.h"
#include "protobuf/kyrin_key.pb.h"

namespace kyrin {
namespace common {

using namespace std;
using namespace kyrin::io;

static KyrinLog *logger = KyrinLog::get_instance();
bool KyrinRsaHelper::examine_legality_from_user(string &issuer, string &digest, string &signature)
{
    string public_key;
    if (!m_cache.get(issuer, public_key) && !fetch_public_key(issuer, public_key)) {
        logger->log("RsaHelper", ("Can't fetch" + issuer).c_str());
        return false;
    }
    return examine_legality(public_key, digest, signature);
}

bool KyrinRsaHelper::examine_legality(string &public_key, string &digest, string &signature)
{
    string plaintext = "";
    if (crypto::public_decrypt(signature, public_key, plaintext) && 
        plaintext == digest) {
        return true;
    }
    return false;
}

bool KyrinRsaHelper::fetch_public_key(string &issuer, string &public_key)
{
    string postdata;
    if (!KyrinHttpClient::make_request_post(m_host, m_port, "/GetKyrinKey", public_key, issuer)) {
        return false;
    }
    return true;
}

} /* kyrin */
} /* common */
