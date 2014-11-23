#ifndef KYRINBOX_SRC_SERVER_KYRIN_BASE_SERVER_H_
#define KYRINBOX_SRC_SERVER_KYRIN_BASE_SERVER_H_

#include <event2/event.h>
#include <event2/http.h>

namespace kyrin {
namespace server {

class KyrinBaseServer {
public:
    bool server_initialize(const char *address, int port);
    virtual bool server_set_processor() = 0;
    bool server_run();
    bool server_free();
    
private:
    event_base *server_event_base;
    evhttp     *server_http_server;
};

} /* end of server */
} /* end of kyrin */

#endif /* end of define guard KYRINBOX_SRC_SERVER_KYRIN_BASE_SERVER_H_ */
