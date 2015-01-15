#include "kyrin_http_client.h"

namespace kyrin {
namespace io {

/*
 * Warning: This class which for http requesting should called carefully
 * For event base would not break the loop automaticly, every callback for this use should call event_base_loopbreak  or resource blackhole
 */
void KyrinHttpClient::make_ev_request_get(void(*request_done_callback)(struct evhttp_request *, void *), const char *host, uint32_t port,  const char *uri, int timeout)
{
    event_base *base = event_base_new();
    evhttp_connection *connection = evhttp_connection_base_new(base, NULL, host, port);
    evhttp_request *request = evhttp_request_new(request_done_callback, base);

    evhttp_make_request(connection, request, EVHTTP_REQ_GET, uri);
    evhttp_connection_set_timeout(request->evcon, timeout);
    event_base_dispatch(base);
    return ;
}

} /* io */
} /* kyrin */
