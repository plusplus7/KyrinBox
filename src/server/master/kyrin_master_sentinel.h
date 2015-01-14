#ifndef KYRINBOX_SRC_SERVER_MASTER_KYRIN_MASTER_SENTINEL_H_
#define KYRINBOX_SRC_SERVER_MASTER_KYRIN_MASTER_SENTINEL_H_

#include <vector>
#include "common/kyrin_lock.h"
#include <string>

namespace kyrin {
namespace server {

struct KyrinMachineInfo {
    char ip[15];
    int  port;
};

enum KyrinMasterRole{
    role_leader = 1,
    role_follower = 2
};

class KyrinMasterSentinel {
public:
    bool start_sentinel(char *filename);
    bool get_status(std::string &message) {
        message = "ahah";
        return true;
    }

private:
    bool read_config(char *filename);

    KyrinMasterRole m_role;
    std::vector<KyrinMachineInfo> m_masters;
    kyrin::common::KyrinMutex m_lock;
};

} /* server */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_MASTER_KYRIN_MASTER_SENTINEL_H_ */
