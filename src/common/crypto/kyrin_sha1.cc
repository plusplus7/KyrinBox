#include "kyrin_sha1.h"
#include <openssl/sha.h>

using namespace std;

namespace kyrin {
namespace common {
namespace crypto {

bool kyrin_sha1(string &data, string &value)
{
    unsigned char md[SHA_DIGEST_LENGTH];
    SHA1((const unsigned char *)data.c_str(), data.length(), md);
    value.assign((const char *)md, SHA_DIGEST_LENGTH);
    return true;
}

}
}
}
