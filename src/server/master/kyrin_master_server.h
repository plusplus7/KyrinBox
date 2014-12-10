#ifndef KYRINBOX_SRC_SERVER_KYRIN_MASTER_SERVER_H_
#define KYRINBOX_SRC_SERVER_KYRIN_MASTER_SERVER_H_

#include "server/kyrin_base_server.h"

namespace kyrin {
namespace server {

class KyrinMasterServer : public KyrinBaseServer {
public:
    bool server_set_processor(evhttp *server);
};

} /* server */

} /* kyrin */

#endif /* end of include guard KYRINBOX_SRC_SERVER_KYRIN_MASTER_SERVER_H_ */
