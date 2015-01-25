#ifndef KYRINBOX_SRC_SERVER_KYRIN_BASE_SERVER_H_
#define KYRINBOX_SRC_SERVER_KYRIN_BASE_SERVER_H_

#include <pthread.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <string>

namespace kyrin {
namespace server {

struct KyrinServerWorkerInfo {
    event_base *server_evbase;
    evhttp *server_evhttp;
    pthread_t server_thread_t;
};

class KyrinBaseServer {
public:
    bool server_run(uint32_t threads = 1);
    virtual bool server_set_processor(evhttp *server, int thread_code) = 0;
    static void *server_thread_func(void *arg);
    bool server_free();

    bool server_initialize_kyrin_server_socket(int &listen_fd, int port, uint32_t backlog);
    bool server_set_evhttp_accept_socket(evhttp *server_evhttp, int listen_fd);
    bool server_put_callback(evhttp *server, const char *path, void(*cb)(evhttp_request *, void *), void *cb_arg);
    bool server_send_reply_ok(evhttp_request *req, std::string &msg);
    bool server_get_postdata(evhttp_request *req, std::string &post_data);

private:
    int server_listen_fd;
    uint32_t server_thread_count;
    KyrinServerWorkerInfo *server_worker_info;
};

} /* end of server */
} /* end of kyrin */

#endif /* end of define guard KYRINBOX_SRC_SERVER_KYRIN_BASE_SERVER_H_ */
