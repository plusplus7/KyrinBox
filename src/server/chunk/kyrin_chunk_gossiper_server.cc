#include <unistd.h>
#include <iostream>
#include "kyrin_chunk_gossiper_server.h"
#include "server/base/kyrin_base_server.h"
#include "common/crypto/kyrin_base64.h"

namespace kyrin {
namespace server {

using namespace std;

static void get_gossiper_status_handler(evhttp_request *req, void *arg)
{
    ((KyrinChunkGossiperServer *) arg)->handle_request(req);
}

bool KyrinChunkGossiperServer::server_initialize(KyrinChunkGossiper *gossiper, int port, int backlog)
{
    if (!server_initialize_kyrin_server_socket(server_listen_fd, port, backlog)) {
        return false;
    }
    m_gossiper = gossiper;
    return true;
}

bool KyrinChunkGossiperServer::server_set_processor(evhttp *server, int thread_code)
{
    server_put_callback(server, "/get_status", get_gossiper_status_handler, this);
    server_set_evhttp_accept_socket(server, server_listen_fd);
    return true;
}

bool KyrinChunkGossiperServer::server_start()
{
    server_run(1);
    return true;
}

bool KyrinChunkGossiperServer::server_close()
{
    KyrinBaseServer::server_free();
    close(server_listen_fd);
    return true;
}

bool KyrinChunkGossiperServer::handle_request(evhttp_request *req)
{
    string request_body = "";
    string reply = "";

    if (!server_get_postdata(req, request_body)) {
        reply = "Can't read post";
        server_send_reply_bad(req, reply);
        return false;
    }
    request_body = common::crypto::base64_decode(request_body);
    kyrinbox::server::ChunkClusterGossipData send;
    send.ParseFromString(request_body);
    m_gossiper->check_commons(send);

    m_gossiper->get_status_string(reply);
    reply = common::crypto::base64_encode((unsigned char const*)reply.c_str(), reply.length());
    server_send_reply_ok(req, reply);
    return true;
}

} /* server */
} /* kyrin */
