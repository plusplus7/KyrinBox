#include "kyrin_chunk_gossiper.h" 

using namespace kyrin::server;

int main(int argc, char **argv)
{
    KyrinChunkGossiper *gossiper = new KyrinChunkGossiper();
    gossiper->start_gossip();
    while (true);
}
