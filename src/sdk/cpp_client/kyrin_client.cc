#include "kyrin_client.h"
#include "io/kyrin_http_client.h"

namespace kyrin {
namespace sdk {

using namespace kyrin::io;

bool KyrinClient::make_request(const char *host, uint32_t port, RequestData &data, std::string &response)
{
    return KyrinHttpClient::make_request_post(host, port, data.uri.c_str(), response, data.postdata, data.timeout); 
}

} /* kyrin*/
} /* sdk */
