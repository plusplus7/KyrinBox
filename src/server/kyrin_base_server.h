#ifndef KYRINBOX_SRC_SERVER_KYRIN_BASE_SERVER_H_
#define KYRINBOX_SRC_SERVER_KYRIN_BASE_SERVER_H_

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <string>

namespace kyrin {
namespace server {

class KyrinBaseServer {
public:
    bool server_initialize(const char *address, int port);
    virtual bool server_set_processor() = 0;
    bool server_run();
    bool server_free();

    bool server_put_callback(const char *path, void(*cb)(evhttp_request *, void *), void *cb_arg);
    bool server_send_reply_ok(evhttp_request *req, std::string &msg);

private:
    event_base *server_event_base;
    evhttp     *server_http_server;
};

} /* end of server */
} /* end of kyrin */

#endif /* end of define guard KYRINBOX_SRC_SERVER_KYRIN_BASE_SERVER_H_ */
