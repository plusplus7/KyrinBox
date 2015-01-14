#include "kyrin_master_sentinel.h"
#include "server/base/kyrin_base_server.h"
#include "kyrin_master_sentinel_server.h"
#include "common/kyrin_log.h"
#include <fstream>

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;
using namespace kyrin::io;

static KyrinLog *logger = KyrinLog::get_instance();
static void *sentinel_thread_func(void *arg)
{
    KyrinMasterSentinel *sentinel = (KyrinMasterSentinel *) arg;
    KyrinMasterSentinelServer *server = new KyrinMasterSentinelServer();
    if (!server->server_initialize(sentinel, 7777, 128*8)) {
        logger->log("sentinel_thread_func", "start_server failed...");
        return NULL;
    }
    server->server_start();
    server->server_close();
    return NULL;
}

static void *sentinel_sync_func(void *arg)
{
    return NULL;
}

bool KyrinMasterSentinel::start_sentinel(char *filename)
{
    //read_config(filename);

    pthread_t sentinel_thread_t;
    pthread_t sentinel_sync_t;

    if (pthread_create(&sentinel_thread_t, NULL, sentinel_thread_func, (void *)this)) {
        return false;    
    }
    if (pthread_create(&sentinel_sync_t, NULL, sentinel_sync_func, (void *)this)) {
        return false;
    }
    return true;
}

bool KyrinMasterSentinel::read_config(char *filename)
{
    ifstream file(filename);
    int count;
    file >> count;
    m_masters.clear();
    for (int i=0; i<count; i++) {
        m_masters.push_back(KyrinMachineInfo());
        file >> m_masters[i].ip >> m_masters[i].port;
    }
    return true;
}

} /* server */
} /* kyrin */
