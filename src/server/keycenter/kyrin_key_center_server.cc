#include <unistd.h>
#include "common/kyrin_log.h"
#include "kyrin_key_center_server.h"
#include "common/configs/kyrin_key_center_config.h"

namespace kyrin {
namespace server {

using namespace kyrin::common;

static KyrinLog *logger = KyrinLog::get_instance();

static void get_kyrin_key_handler(evhttp_request *req, void *arg)
{
    ((KyrinKeyCenterServer *) arg)->get_get_kyrin_key_request_handler()->handle_request((KyrinKeyCenterServer *)arg, req);
}

static void set_kyrin_key_handler(evhttp_request *req, void *arg)
{
    ((KyrinKeyCenterServer *) arg)->get_set_kyrin_key_request_handler()->handle_request((KyrinKeyCenterServer *)arg, req);
}

bool KyrinKeyCenterServer::server_initialize()
{
    configs::KyrinKeyCenterConfig *config = new configs::KyrinKeyCenterConfig();
    config->read_config_file((char *)"kyrinbox_config.json");
    if (!server_initialize_kyrin_server_socket(get_key_fd,
         config->get_key_port(),
         config->get_key_backlog())) {
        delete config;
        return false;
    }

    if (!server_initialize_kyrin_server_socket(set_key_fd,
         config->set_key_port(),
         config->set_key_backlog())) {
        delete config;
        return false;
    }

    get_kyrin_key_request_handler = new GetKyrinKeyRequestHandler((char *)config->redis_host().c_str(), config->redis_port());
    set_kyrin_key_request_handler = new SetKyrinKeyRequestHandler((char *)config->redis_host().c_str(), config->redis_port());
    delete config;
    return true;
}

bool KyrinKeyCenterServer::server_free()
{
    KyrinBaseServer::server_free();
    close(get_key_fd);
    close(set_key_fd);
    delete get_kyrin_key_request_handler;
    delete set_kyrin_key_request_handler;
    return true;
}

bool KyrinKeyCenterServer::server_start()
{
    server_run(2);
    return true;
}

bool KyrinKeyCenterServer::server_set_processor(evhttp *server, int thread_code)
{
    if (thread_code) {
        server_put_callback(server, "/SetKyrinKey", set_kyrin_key_handler, this);
        server_set_evhttp_accept_socket(server, set_key_fd);
    } else {
        server_put_callback(server, "/GetKyrinKey", get_kyrin_key_handler, this);
        server_set_evhttp_accept_socket(server, get_key_fd);
    }
    return true;
}

} /* server */
} /* kyrin */
