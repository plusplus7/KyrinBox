#ifndef KYRINBOX_SRC_COMMON_KYRIN_LEXICOGRAPHICALLY_HELPER_H_
#define KYRINBOX_SRC_COMMON_KYRIN_LEXICOGRAPHICALLY_HELPER_H_

#include <string>

namespace kyrin {
namespace common {

class KyrinLexicographicallyHelper {
public:
    static void get_zero(std::string &s) {
        const char zero[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        s.assign(zero, 8);
    }

    static bool add_one(std::string &s) {
        for (int i=7; i>=0; i--) {
            if (uint32_t(s[i]) != 0xFFFFFFFF) {
                s[i]++;
                return true;
            } else {
                s[i] = 0;
            }
        }
        return false;
    }
};

} /* common */
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_KYRIN_LEXICOGRAPHICALLY_HELPER_H_ */
