#include "kyrin_rsa.h"
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>

namespace kyrin {
namespace common {
namespace crypto {

using namespace std;

static RSA *create_rsa(unsigned char *key, bool is_public)
{
    RSA *rsa = NULL;
    BIO *keybio = NULL;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio == NULL) {
        return NULL;
    }
    if (is_public) {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
    } else {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
    }
    return rsa;
}

bool public_encrypt(string &plaintext, string &key, string &encrypted)
{
    RSA *rsa = create_rsa((unsigned char *)key.c_str(), true);
    if (rsa == NULL) {
        return false;
    }
    unsigned char encrypt[4098]={};  /* FIXME: hardcode 4098*/
    int length = RSA_public_encrypt(plaintext.length(), (const unsigned char *)plaintext.c_str(), encrypt, rsa, RSA_PKCS1_PADDING);
    if (length == -1) {
        return false;
    }
    encrypted.assign((const char *)encrypt, length);
    return true;
}

bool public_decrypt(string &ciphertext, string &key, string &decrypted)
{
    RSA *rsa = create_rsa((unsigned char *)key.c_str(), true);
    if (rsa == NULL) {
        return false;
    }
    unsigned char decrypt[4098]={}; 
    int length = RSA_public_decrypt(ciphertext.length(), (const unsigned char *)ciphertext.c_str(), decrypt, rsa, RSA_PKCS1_PADDING);
    if (length == -1) {
        return false;
    }
    decrypted.assign((const char *)decrypt, length);
    return true;
}

bool private_encrypt(string &plaintext, string &key, string &encrypted)
{
    RSA *rsa = create_rsa((unsigned char *)key.c_str(), false);
    if (rsa == NULL) {
        return false;
    }
    unsigned char encrypt[4098]={}; 
    int length = RSA_private_encrypt(plaintext.length(), (const unsigned char *)plaintext.c_str(), encrypt, rsa, RSA_PKCS1_PADDING);
    if (length == -1) {
        return false;
    }
    encrypted.assign((const char *)encrypt, length);
    return true;
}

bool private_decrypt(string &ciphertext, string &key, string &decrypted)
{
    RSA *rsa = create_rsa((unsigned char *)key.c_str(), false);
    if (rsa == NULL) {
        return false;
    }
    unsigned char decrypt[4098]={}; 
    int length = RSA_private_decrypt(ciphertext.length(), (const unsigned char *)ciphertext.c_str(), decrypt, rsa, RSA_PKCS1_PADDING);
    if (length == -1) {
        return false;
    }
    decrypted.assign((const char *)decrypt, length);
    return true;
}

} /* crypto */
} /* common */
} /* kyrin */
