#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <string>
#include <utility>

namespace kyrin {
namespace io {

class TcpServerWrapper {
public:
    bool initialize(int port, std::vector< std::pair<uint32_t, int> > &clients);
    bool send_message(int id, std::string &message);

private:
    int m_listen_socket;
    std::map<uint32_t, int> m_socket_map;
};

} /* io */
} /* kyrin */
