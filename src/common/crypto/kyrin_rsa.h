#ifndef KYRINBOX_SRC_COMMON_CRYPTO_KYRIN_RSA_H_
#define KYRINBOX_SRC_COMMON_CRYPTO_KYRIN_RSA_H_

#include <string>

namespace kyrin {
namespace common {
namespace crypto {

bool public_encrypt(std::string &plaintext, std::string &key, std::string &encrypted);
bool public_decrypt(std::string &ciphertext, std::string &key, std::string &decrypted);
bool private_encrypt(std::string &plaintext, std::string &key, std::string &encrypted);
bool private_decrypt(std::string &ciphertext, std::string &key, std::string &decrypted);

} /* crypto */
} /* common */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_CRYPTO_KYRIN_RSA_H_ */
