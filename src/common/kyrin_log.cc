#include "kyrin_log.h"
#include <time.h>
#include "common/kyrin_constants.h"

namespace kyrin {
namespace common {

KyrinLog* KyrinLog::kyrin_log = NULL;

KyrinLog* KyrinLog::get_instance()
{
    if (kyrin_log == NULL)
        kyrin_log = new KyrinLog();
    return kyrin_log;
}

KyrinLog::KyrinLog()
{
    m_log_fp = fopen("kyrinbox.log", "a"); /* FIXME: rotate */
}

KyrinLog::~KyrinLog()
{
    fclose(m_log_fp);
}

void KyrinLog::log(const char *key, const char *msg)
{
    time_t timep;
    time(&timep);
    char *t_time = asctime(gmtime(&timep));
    t_time[strlen(t_time)-1] = '\0';
    fprintf(m_log_fp, "[%s] [%s] %s\n", t_time, key, msg);
    fflush(m_log_fp);
}

} /* common */
} /* kyrin */
