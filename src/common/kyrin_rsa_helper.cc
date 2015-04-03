#include "kyrin_rsa_helper.h"
#include "kyrin_log.h"
#include "common/crypto/kyrin_rsa.h"

namespace kyrin {
namespace common {

using namespace std;

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
    
}

} /* kyrin */
} /* common */
