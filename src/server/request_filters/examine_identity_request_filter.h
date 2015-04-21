#ifndef KYRINBOX_SRC_SERVER_REQUEST_FILTERS_EXAMINE_IDENTITY_REQUEST_FILTER_H_
#define KYRINBOX_SRC_SERVER_REQUEST_FILTERS_EXAMINE_IDENTITY_REQUEST_FILTER_H_

#include "kyrin_base_request_filter.h"
#include "server/base/kyrin_base_server.h"
#include "common/kyrin_rsa_helper.h"

namespace kyrin {
namespace server {

class ExamineIdentityRequestFilter : public KyrinBaseRequestFilter {
public:
    ExamineIdentityRequestFilter(const char *host, int port) : KyrinBaseRequestFilter() {
        m_rsa_helper = new kyrin::common::KyrinRsaHelper(host, port);
    }

    ~ExamineIdentityRequestFilter() {
        delete m_rsa_helper;
    }
    bool filter_request(KyrinBaseServer *server, evhttp_request *req);

private:
    kyrin::common::KyrinRsaHelper *m_rsa_helper; 
};

} /* kyrin */
} /* server */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_REQUEST_FILTERS_EXAMINE_IDENTITY_REQUEST_FILTER_H_ */
