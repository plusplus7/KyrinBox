#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include "kyrin_master_server.h"
#include "common/kyrin_log.h"
#include "common/kyrin_config.h"
#include "common/kyrin_constants.h"

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

static KyrinLog *logger = KyrinLog::get_instance();

bool KyrinMasterServer::server_initialize()
{
    if (!server_initialize_kyrin_server_socket(reader_listen_fd,
        atoi(KyrinConfig::get_instance()->get_config(constants::k_json_master_reader_port).c_str()),
        atoi(KyrinConfig::get_instance()->get_config(constants::k_json_master_reader_backlog).c_str()))) {
        return false;
    }

    if (!server_initialize_kyrin_server_socket(processor_listen_fd,
        atoi(KyrinConfig::get_instance()->get_config(constants::k_json_master_processor_port).c_str()),
        atoi(KyrinConfig::get_instance()->get_config(constants::k_json_master_processor_backlog).c_str()))) {
        return false;
    }

    return true;
}

bool KyrinMasterServer::server_free()
{
    KyrinBaseServer::server_free();
    close(reader_listen_fd);
    close(processor_listen_fd);
    return true;
}

bool KyrinMasterServer::server_start()
{
    server_run(atoi(KyrinConfig::get_instance()->get_config(constants::k_json_master_server_threads).c_str()));
    return true;
}

bool KyrinMasterServer::server_set_processor(evhttp *server, int thread_code)
{
    return true;
}

} /* server */
} /* kyrin */
