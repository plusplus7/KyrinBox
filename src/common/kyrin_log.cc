#include <iostream>
#include "kyrin_log.h"

namespace kyrin {
namespace common {

using namespace std;

KyrinLog* KyrinLog::kyrin_log = NULL;
KyrinLog* KyrinLog::get_instance() {
    if (kyrin_log == NULL)
        kyrin_log = new KyrinLog();
    return kyrin_log;
}

void KyrinLog::log(const char *key, const char *msg) {
    cout<<"["<<key<<"]: "<<msg<<endl;
}

} /* common */
} /* kyrin */
