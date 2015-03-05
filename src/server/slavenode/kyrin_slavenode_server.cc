#include <unistd.h>
#include "kyrin_slavenode_server.h"
#include "common/configs/kyrin_slavenode_config.h"

namespace kyrin {
namespace server {

using namespace kyrin::common;

static void download_file_handler(evhttp_request *req, void *arg)
{
}

bool KyrinSlavenodeServer::server_initialize(KyrinSlavenodeSync *sync)
{
    if (sync == NULL) {
        return false;
    }
    m_sync = sync;
    configs::KyrinSlavenodeConfig *config = new configs::KyrinSlavenodeConfig();
    config->read_config_file((char *)"kyrinbox_config.json");
    if (!server_initialize_kyrin_server_socket(download_file_fd,
         config->download_file_port(),
         config->download_file_backlog())) {
        delete config;
        return false;
    }

    delete config;
    return true;
}

bool KyrinSlavenodeServer::server_free()
{
    KyrinBaseServer::server_free();
    close(download_file_fd);
    return true;
}

bool KyrinSlavenodeServer::server_start()
{
    server_run(4); /* FIXME */
    return true;
}

bool KyrinSlavenodeServer::server_set_processor(evhttp *server, int thread_code)
{
    server_put_callback(server, "/DownloadFile", download_file_handler, this);
    server_set_evhttp_accept_socket(server, download_file_fd);
    return true;
}

} /* server */
} /* kyrin */
