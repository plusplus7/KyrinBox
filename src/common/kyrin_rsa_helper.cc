#include "kyrin_rsa_helper.h"
#include "kyrin_log.h"
#include "common/crypto/kyrin_rsa.h"
#include "io/kyrin_http_client.h"
#include "common/crypto/kyrin_base64.h"
#include "protobuf/kyrin_key.pb.h"
#include <iostream>

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
    logger->log("rsa helper", (plaintext+ "->plaintext, " + signature + "->signature, examine failed...").c_str());
    return false;
}

bool KyrinRsaHelper::fetch_public_key(string &issuer, string &public_key)
{
    logger->log("rsa helper", (issuer + " " + public_key).c_str());
    kyrinbox::api::GetKyrinKeyRequest request;
    request.set_key_id(issuer);
    string postdata;
    request.SerializeToString(&postdata);
    postdata = crypto::base64_encode((unsigned char const*)postdata.c_str(), postdata.length());
    if (!KyrinHttpClient::make_request_post(m_host, m_port, "/GetKyrinKey", public_key, postdata)) {
        return false;
    }
    return true;
}

} /* kyrin */
} /* common */
