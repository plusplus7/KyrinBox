#include "kyrin_base_server.h"
#include "common/kyrin_log.h"
#include <iostream>
#include <string>
#include <fcntl.h>
#include <errno.h>

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;
using namespace kyrin::io;

static KyrinLog *logger = KyrinLog::get_instance();
bool KyrinBaseServer::server_run(const char *address, int port, uint32_t threads, uint32_t backlog)
{
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr;

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    int reuseaddr = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuseaddr, sizeof(int)) < 0) {
        logger->log("server_initialize", "set socket option failed...");
        return false;
    }

    if (bind(listen_fd, (sockaddr *)&server_addr, sizeof(sockaddr)) < 0) {
        logger->log("server_initialize", "bind failed...");
        return false;
    }

    if (listen(listen_fd, backlog) < 0) {
        logger->log("server_initialize", "listen failed...");
        return false;
    }

    int flags;
    if ((flags = fcntl(listen_fd, F_GETFL, 0)) < 0 || fcntl(listen_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        logger->log("server_initialize", "fcntl failed...");
        return false;
    }

    thread_count = threads;
    server_worker_info = new KyrinServerWorkerInfo [thread_count];
    for (int i=0; i<threads; i++) {
        server_worker_info[i].server_evbase = event_base_new();
        server_worker_info[i].server_evhttp = evhttp_new(server_worker_info[i].server_evbase);
        server_set_processor(server_worker_info[i].server_evhttp, i);

        evhttp_accept_socket(server_worker_info[i].server_evhttp, listen_fd);

        pthread_create(&(server_worker_info[i].server_thread_t),
                       NULL,
                       KyrinBaseServer::server_thread_func,
                       (void *)(server_worker_info+i));
    }
    server_database_wrapper = new KyrinDatabaseWrapper();
    return true;
}

void *KyrinBaseServer::server_thread_func(void *arg) {
    event_base_dispatch(((KyrinServerWorkerInfo*)arg)->server_evbase);
    return NULL;
}

bool KyrinBaseServer::server_free() {
    for (int i=0; i<thread_count; i++) {
        pthread_join(server_worker_info[i].server_thread_t, NULL);
        evhttp_free(server_worker_info[i].server_evhttp);
        event_base_free(server_worker_info[i].server_evbase);
    }
    delete server_database_wrapper;
    delete [] server_worker_info;
    return true;
}

bool KyrinBaseServer::server_put_callback(evhttp *server, const char *path, void(*cb)(struct evhttp_request *, void *), void *cb_arg) {
    evhttp_set_cb(server, path, cb, cb_arg);
    return true;
}

bool KyrinBaseServer::server_send_reply_ok(evhttp_request *req, string &msg) {
    evbuffer *buf = evbuffer_new();
    if (buf == NULL) {
        logger->log("server_send_reply_ok", "new buf failed");
        return false;
    }
    evbuffer_add_printf(buf, "%s", msg.c_str());
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
    return true;
}

bool KyrinBaseServer::server_get_postdata(evhttp_request *req, string &post_data) {
    size_t post_size = evbuffer_get_length(evhttp_request_get_input_buffer(req));
    if (/* likely */post_size > 0) {
        char _buf[1024]; /* FIXME */
        memcpy(_buf, evbuffer_pullup(evhttp_request_get_input_buffer(req), -1), post_size);
        post_data.assign(_buf, post_size);
        return true;
    }
    return false;
}

KyrinDatabaseWrapper* KyrinBaseServer::server_get_database()
{
    return server_database_wrapper;
}

} /* server */
} /* kyrin */
