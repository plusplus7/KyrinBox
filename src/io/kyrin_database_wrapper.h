#ifndef KYRINBOX_SRC_IO_KYRIN_DATABASE_WRAPPER_H_
#define KYRINBOX_SRC_IO_KYRIN_DATABASE_WRAPPER_H_

#include "leveldb/db.h"
#include <iostream>
#include <string>

namespace kyrin {
namespace io {

class KyrinDatabaseWrapper {
public:
    explicit KyrinDatabaseWrapper(std::string &db_path);
    explicit KyrinDatabaseWrapper(const char *db_path);
    ~KyrinDatabaseWrapper();

    bool put(const std::string &key, const std::string &value);
    bool get(const std::string &key, std::string &value);
    bool remove(const std::string &key);
    bool exist(const std::string &key);
    bool fetch_last_key(std::string &key);

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
