#ifndef KYRINBOX_SRC_IO_KYRIN_DATABASE_WRAPPER_H_
#define KYRINBOX_SRC_IO_KYRIN_DATABASE_WRAPPER_H_

#include "leveldb/db.h"
#include <string>

namespace kyrin {
namespace io {

class KyrinDatabaseWrapper {
public:
    KyrinDatabaseWrapper(std::string &db_name);
    ~KyrinDatabaseWrapper();

    bool database_put(const std::string &key, const std::string &value);
    bool database_get(const std::string &key, std::string &value);

private:
    bool database_connect();
    bool database_disconnect();
    bool database_reconnect();

    std::string database_path;
    leveldb::DB *database_connection;
    leveldb::Options database_options;
};

} /* io */
} /* kyrin */

#endif /* end of define guard: KYRINBOX_SRC_IO_KYRIN_DATABASE_WRAPPER_H_ */
