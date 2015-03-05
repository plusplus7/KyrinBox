#include "kyrin_slavenode_sync.h"
#include "kyrin_slavenode_server.h"

using namespace kyrin::server;

int main()
{
    KyrinSlavenodeSync *sync = new KyrinSlavenodeSync();
    sync->start_sync();
    KyrinSlavenodeServer *server = new KyrinSlavenodeServer();
    server->server_initialize(sync);
    server->server_start();
    server->server_free();
    delete server;
    return 0;
}
