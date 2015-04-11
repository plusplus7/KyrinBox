#ifndef KYRINBOX_SRC_COMMON_KYRIN_RSA_HELPER_H_
#define KYRINBOX_SRC_COMMON_KYRIN_RSA_HELPER_H_

#include "common/kyrin_lru_cache.h"

namespace kyrin {
namespace common {

class KyrinRsaHelper {
public:
    KyrinRsaHelper(const char *host, int port, uint32_t size = 65535) : m_host(host), m_port(port), m_cache(size) {}
    bool examine_legality_from_user(std::string &issuer, std::string &digest, std::string &signature);
    static bool examine_legality(std::string &public_key, std::string &digest, std::string &signature);

private:
    bool fetch_public_key(std::string &issuer, std::string &public_key);
    KyrinLruCache<std::string, std::string> m_cache;
    const char *m_host;
    int m_port;

};

} /* common */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_KYRIN_RSA_HELPER_H_ */
