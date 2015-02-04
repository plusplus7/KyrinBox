#ifndef KYRINBOX_SRC_COMMON_KYRIN_LOCK_H_
#define KYRINBOX_SRC_COMMON_KYRIN_LOCK_H_
#include <pthread.h>

namespace kyrin {
namespace common {

class KyrinMutex {
public:
    KyrinMutex()
    {
        pthread_mutex_init(&m_mutex, NULL);
    }

    ~KyrinMutex()
    {
        pthread_mutex_destroy(&m_mutex);
    }

    int lock()
    {
        return pthread_mutex_lock(&m_mutex);
    }

    int unlock()
    {
        return pthread_mutex_unlock(&m_mutex);
    }

    int trylock()
    {
        return pthread_mutex_trylock(&m_mutex);
    }

private:
    pthread_mutex_t m_mutex;

};

} /* common */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_KYRIN_LOCK_H_ */
