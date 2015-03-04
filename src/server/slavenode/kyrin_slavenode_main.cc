#include "kyrin_slavenode_sync.h"

using namespace kyrin::server;

int main()
{
    KyrinSlavenodeSync *sync = new KyrinSlavenodeSync();
    sync->start_sync();

    while(true);
    //KyrinSlavenodeServer *server = new KyrinSlavenodeServer();
    //server->server_initialize();
    //server->server_start();
    //server->server_free();
    //delete server;
    return 0;
}
