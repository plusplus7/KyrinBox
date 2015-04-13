#ifndef KYRINBOX_SRC_SDK_KYRIN_SDK_H_
#define KYRINBOX_SRC_SDK_KYRIN_SDK_H_

#include <string>

namespace kyrin {
namespace sdk {

struct RequestData {
    std::string uri;
    std::string postdata;
    int         timeout;
    RequestData() {}
    RequestData(std::string &_uri, std::string &_postdata, int _timeout = 5): uri(_uri), postdata(_postdata), timeout(_timeout){}
    ~RequestData() {}
};

class KyrinClient {
public:
    bool make_request(const char *host, uint32_t port, RequestData &data, std::string &response);
};

} /* sdk */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SDK_KYRIN_SDK_H_ */
