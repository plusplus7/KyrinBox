#ifndef KYRINBOX_SRC_SERVER_KYRIN_BASE_SERVER_H_
#define KYRINBOX_SRC_SERVER_KYRIN_BASE_SERVER_H_

#include <pthread.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <string>
#include "io/kyrin_database_wrapper.h"

namespace kyrin {
namespace server {

class KyrinBaseServer {
public:
    bool server_run(const char *address, int port, uint32_t threads = 1, uint32_t backlog = 1024);
    virtual bool server_set_processor(evhttp *server) = 0;
    static void *server_thread_func(void *arg);
    bool server_free();

    bool server_put_callback(evhttp *server, const char *path, void(*cb)(evhttp_request *, void *), void *cb_arg);
    bool server_send_reply_ok(evhttp_request *req, std::string &msg);
    bool server_get_postdata(evhttp_request *req, std::string &post_data);
    kyrin::io::KyrinDatabaseWrapper* server_get_database();

private:
    uint32_t thread_count;
    pthread_t *thread_t;
    kyrin::io::KyrinDatabaseWrapper *server_database_wrapper;
};

} /* end of server */
} /* end of kyrin */

#endif /* end of define guard KYRINBOX_SRC_SERVER_KYRIN_BASE_SERVER_H_ */
