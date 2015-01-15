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
    bool get_vote(std::string &message);
    bool set_vote(uint64_t vote);
    bool get_status(KyrinMasterStatus &message);
    bool set_status(KyrinMasterStatus status);
    uint64_t get_kbid() {
        return m_kbid;
    }
    int get_listen_port() {
        return m_masters[m_kbid].port;
    }
    std::vector<KyrinMachineInfo> get_all_masters() {
        return m_masters;
    }

private:
    bool read_config(char *filename);

    KyrinMasterStatus m_status;
    uint64_t m_kbid;
    uint64_t m_leader;
    uint64_t m_epoch;
    std::vector<KyrinMachineInfo> m_masters;
    kyrin::common::KyrinMutex m_status_lock;
    kyrin::common::KyrinMutex m_vote_lock;
};

} /* server */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_MASTER_KYRIN_MASTER_SENTINEL_H_ */
