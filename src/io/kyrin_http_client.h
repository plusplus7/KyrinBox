#include <string>
#include <evhttp.h>
#include <event2/http.h>

namespace kyrin {
namespace io {

class KyrinHttpClient {
public:
    static void make_ev_request_get(void(*cb)(struct evhttp_request *, void *), const char *host, uint32_t port,  const char *uri, int timeout = 5);
    static bool make_request_get(const char *host, uint32_t port, const char *uri, std::string &response, int timeout = 5);
    static bool make_request_post(const char *host, uint32_t port, const char *uri, std::string &response, std::string &postdata, int timeout = 5);
};

} /* io */
} /* kyrin */
