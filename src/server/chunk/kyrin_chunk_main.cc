#include "kyrin_chunk_gossiper.h" 
#include "kyrin_chunk_server.h"
#include "unistd.h"

using namespace kyrin::server;

int main(int argc, char **argv)
{
    KyrinChunkGossiper *gossiper = new KyrinChunkGossiper();
    gossiper->start_gossip();

    KyrinChunkServer *server = new KyrinChunkServer();
    server->server_initialize(gossiper);
    server->server_start();
    server->server_free();
    delete gossiper;
    delete server;
    return 0;
}
