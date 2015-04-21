#ifndef KYRINBOX_SRC_SERVER_REQUEST_FILTERS_KYRIN_BASE_REQUEST_FILTER_H_
#define KYRINBOX_SRC_SERVER_REQUEST_FILTERS_KYRIN_BASE_REQUEST_FILTER_H_
#include "server/base/kyrin_base_server.h"

namespace kyrin {
namespace server {

class KyrinBaseRequestFilter {
public:
    virtual bool filter_request(KyrinBaseServer *server, evhttp_request *req) = 0;
    KyrinBaseRequestFilter();

}; /* KyrinBaseRequestFilter */

} /* server */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_REQUEST_FILTERS_KYRIN_BASE_REQUEST_FILTER_H_ */
