#include "kyrin_master_server.h"

using namespace kyrin::server;

int main(int argv, char** args) {

    KyrinMasterServer *kyrin_master_server = new KyrinMasterServer();
    kyrin_master_server->server_initialize("127.0.0.1", 7770);
    kyrin_master_server->server_set_processor();
    
    kyrin_master_server->server_run();
    
    kyrin_master_server->server_free();
    return 0;
}
