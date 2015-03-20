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
    void sentinel_sync();

    bool get_status(KyrinMasterStatus &message);
    bool set_status(KyrinMasterStatus status);
    bool get_vote(uint64_t &vote);
    bool set_vote(uint64_t vote);
    bool get_leader(uint32_t &leader);
    bool set_leader(uint32_t leader);
    bool get_epoch(uint64_t &epoch);
    bool set_epoch(uint64_t epoch);
    bool get_vote_ticket(std::string &ticket);

private:
    KyrinMasterStatus m_status;
    uint32_t m_leader;
    uint64_t m_vote;
    uint64_t m_epoch;
    kyrin::common::KyrinMutex m_status_lock;
    kyrin::common::KyrinMutex m_vote_lock;
    kyrin::common::KyrinMutex m_leader_lock;
    kyrin::common::KyrinMutex m_epoch_lock;
};

} /* server */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_SERVER_MASTER_KYRIN_MASTER_SENTINEL_H_ */
