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

enum KyrinMasterStatus{
    k_status_leader = 1,
    k_status_follower = 2,
    k_status_consensus = 3
};

class KyrinMasterSentinel {
public:
    bool start_sentinel(char *filename);
    bool get_status(KyrinMasterStatus &message);

private:
    bool read_config(char *filename);

    KyrinMasterStatus m_status;
    uint32_t m_code;
    std::vector<KyrinMachineInfo> m_masters;
    kyrin::common::KyrinMutex m_status_lock;
};

} /* server */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_MASTER_KYRIN_MASTER_SENTINEL_H_ */
