#ifndef KYRINBOX_SRC_COMMON_KYRIN_LOG_H_
#define KYRINBOX_SRC_COMMON_KYRIN_LOG_H_

namespace kyrin {
namespace common {

class KyrinLog {
public:
    static KyrinLog *get_instance();
    void log(const char *key, const char *msg);

private:
    KyrinLog(){}
    static KyrinLog *kyrin_log;
};

}
}

#endif /* end of define guard: KYRINBOX_SRC_COMMON_KYRIN_LOG_H_ */

