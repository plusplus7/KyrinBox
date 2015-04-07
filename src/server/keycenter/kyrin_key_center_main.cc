#include "kyrin_key_center_server.h"

using namespace kyrin::server;

int main()
{
    KyrinKeyCenterServer *server = new KyrinKeyCenterServer();
    server->server_initialize();
    server->server_start();
    server->server_free();
    delete server;
    return 0;
}
