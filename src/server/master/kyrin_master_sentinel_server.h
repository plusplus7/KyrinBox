#ifndef KYRINBOX_SRC_SERVER_KYRIN_MASTER_SERVER_H_
#define KYRINBOX_SRC_SERVER_KYRIN_MASTER_SERVER_H_

#include "server/base/kyrin_base_server.h"
#include "server/master/kyrin_master_sentinel.h"

namespace kyrin {
namespace server {

class KyrinMasterSentinelServer : public KyrinBaseServer {
public:
    bool server_initialize(KyrinMasterSentinel *sentinel, int port, int backlog); 
    bool server_set_processor(evhttp *server, int thread_code);
    bool server_start();
    bool server_close();
    KyrinMasterSentinel* get_sentinel();

private:
    KyrinMasterSentinel *m_sentinel;
    int server_listen_fd;

};

} /* server */
} /* kyrin */

#endif /* end of include guard KYRINBOX_SRC_SERVER_KYRIN_MASTER_SERVER_H_ */
