#ifndef KYRINBOX_SRC_COMMON_KYRIN_LOG_H_
#define KYRINBOX_SRC_COMMON_KYRIN_LOG_H_

#include <stdio.h>

namespace kyrin {
namespace common {

class KyrinLog {
public:
    static KyrinLog *get_instance();
    void log(const char *key, const char *msg);

private:
    KyrinLog();
    ~KyrinLog();
    static KyrinLog *kyrin_log;
    FILE *m_log_fp;

};

}
}

#endif /* end of define guard: KYRINBOX_SRC_COMMON_KYRIN_LOG_H_ */
