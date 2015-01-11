#include "kyrin_network.h"
#include "common/kyrin_log.h"
#include <iostream>

using namespace std;
using namespace kyrin::common;

namespace kyrin {
namespace io {

static KyrinLog *logger = KyrinLog::get_instance();
bool TcpServerWrapper::initialize(int port, vector< pair<uint32_t, int> > &clients)
{
    m_listen_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(port);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(m_listen_socket, (sockaddr *)&server_sockaddr, sizeof(server_sockaddr)) == -1) {
        logger->log("TcpServerWrapper initialize", "bind error");
        return false;
    }

    if (listen(m_listen_socket, clients.size()) == -1) {
        logger->log("TcpServerWrapper initialize", "listen failed");
        return false;
    }

    // 生成socket map, O(n^2)
    m_socket_map.clear();
    for (int i=0; i<clients.size(); i++) {
        sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);

        int connect_socket = accept(m_listen_socket, (sockaddr*)&client_addr, &length);
        if (connect_socket < 0) {
            logger->log("TcpServerWrapper initialize", "connect_socket error");
            return false;
        }

        uint32_t uint_ip = client_addr.sin_addr.s_addr;
        cout<<uint_ip<<endl;
        for (int j=0; j<clients.size(); j++) {
            if (uint_ip == clients[j].first && m_socket_map.count(clients[j].second) == 0) {
                m_socket_map[clients[j].second] = connect_socket;
                break;
            }
        }
    }

    return true;
}

bool TcpServerWrapper::send_message(int id, string &message)
{
    if (!m_socket_map.count(id))
        return false;
    send(m_socket_map[id], message.c_str(), message.length(), 0);
    return true;
}

} /* io */
} /* kyrin */
