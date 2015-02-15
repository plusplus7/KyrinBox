#ifndef KYRINBOX_SRC_SERVER_MASTER_KYRIN_MASTER_SENTINEL_H_
#define KYRINBOX_SRC_SERVER_MASTER_KYRIN_MASTER_SENTINEL_H_

#include <vector>
#include "common/kyrin_lock.h"
#include <string>

namespace kyrin {
namespace server {

enum KyrinMasterStatus{
    k_status_leader = 1,
    k_status_follower = 2,
    k_status_consensus = 3
};

class KyrinMasterSentinel {
public:
    bool start_sentinel();
    bool get_vote(std::string &message);
    bool set_vote(uint64_t vote, bool next_epoch = false);
    bool get_status(KyrinMasterStatus &message);
    bool set_status(KyrinMasterStatus status);
    uint64_t get_epoch() {
        return m_epoch;
    }

private:
    bool read_config(char *filename);

    KyrinMasterStatus m_status;
    uint64_t m_leader;
    uint64_t m_epoch;
    kyrin::common::KyrinMutex m_status_lock;
    kyrin::common::KyrinMutex m_vote_lock;
};

} /* server */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_MASTER_KYRIN_MASTER_SENTINEL_H_ */
