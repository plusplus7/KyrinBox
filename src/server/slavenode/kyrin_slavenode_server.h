#ifndef KYRINBOX_SRC_SERVER_KYRIN_SLAVENODE_SERVER_H_
#define KYRINBOX_SRC_SERVER_KYRIN_SLAVENODE_SERVER_H_

#include "server/base/kyrin_base_server.h"

namespace kyrin {
namespace server {

class KyrinSlavenodeServer : public KyrinBaseServer {
public:
    bool server_initialize();
    bool server_start();
    bool server_free();
    bool server_set_processor(evhttp *server, int thread_code);

private:
    int download_file_fd;
};

} /* server */
} /* kyrin */
#endif /* end of include guard: KYRINBOX_SRC_SERVER_KYRIN_SLAVENODE_SERVER_H_ */
