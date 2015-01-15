#include <evhttp.h>
#include <event2/http.h>

namespace kyrin {
namespace io {

class KyrinHttpClient {
public:
    static void make_ev_request_get(void(*cb)(struct evhttp_request *, void *), const char *host, uint32_t port,  const char *uri, int timeout = 5);
};

} /* io */
} /* kyrin */
