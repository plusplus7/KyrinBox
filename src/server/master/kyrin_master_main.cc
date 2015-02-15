#include "kyrin_master_server.h"
#include "kyrin_master_sentinel.h"

using namespace kyrin::server;

int main(int argv, char** args)
{
    KyrinMasterSentinel *kyrin_master_sentinel = new KyrinMasterSentinel();
    kyrin_master_sentinel->start_sentinel();
    KyrinMasterServer *kyrin_master_server = new KyrinMasterServer();
    kyrin_master_server->server_initialize(kyrin_master_sentinel);
    kyrin_master_server->server_start();
    kyrin_master_server->server_free();
    delete kyrin_master_sentinel;
    delete kyrin_master_server;
    return 0;
}
