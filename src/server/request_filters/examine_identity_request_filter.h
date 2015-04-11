#ifndef KYRINBOX_SRC_SERVER_REQUEST_FILTERS_EXAMINE_IDENTITY_REQUEST_FILTER_H_
#define KYRINBOX_SRC_SERVER_REQUEST_FILTERS_EXAMINE_IDENTITY_REQUEST_FILTER_H_

#include "kyrin_base_request_filter.h"

namespace kyrin {
namespace server {

class ExamineIdentityRequestFilter : public KyrinBaseRequestFilter {
public:
    ExamineIdentityRequestFilter() : KyrinBaseRequestFilter() {
        m_rsa_helper = new KyrinRsaHelper();
    }

    ~ExamineIdentityRequestFilter() {
        delete m_rsa_helper;
    }
    bool filter_request(KyrinBaseServer *server, evhttp_request *req, std::string &reply, std::string &digest, std::string &issuer);

private:
    KyrinRsaHelper *m_rsa_helper; 
};

} /* kyrin */
} /* server */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_REQUEST_FILTERS_EXAMINE_IDENTITY_REQUEST_FILTER_H_ */
