#include "kyrin_database_wrapper.h"
#include "common/kyrin_log.h"
#include "common/kyrin_macros.h"

namespace kyrin {
namespace io {

using namespace std;
using namespace kyrin::common;

static KyrinLog *logger = KyrinLog::get_instance();
KyrinDatabaseWrapper::KyrinDatabaseWrapper(string &db_path)
{
    database_path = db_path;
    database_connect();
}

KyrinDatabaseWrapper::KyrinDatabaseWrapper(const char *db_path)
{
    database_path.assign(db_path);
    database_connect();
}

KyrinDatabaseWrapper::~KyrinDatabaseWrapper()
{
    database_disconnect();
}

bool KyrinDatabaseWrapper::database_connect()
{
    database_options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(database_options, database_path, &database_connection);
    if (kyrin_unlikely(!status.ok())) {
        logger->log("KyrinDatabaseWrapper", "Open database failed...");
        return false;
    }
    logger->log("KyrinDatabaseWrapper", ("Database opened " + status.ToString()).c_str());
    return true;
}

bool KyrinDatabaseWrapper::database_disconnect()
{
    delete database_connection;
    return true;
}

bool KyrinDatabaseWrapper::database_reconnect()
{
    database_disconnect();
    return database_connect();
}

bool KyrinDatabaseWrapper::put(const string &key, const string &value)
{
    leveldb::Status status = database_connection->Put(leveldb::WriteOptions(), key, value);
    if (kyrin_likely(status.ok())) {
        return true;
    } else {
        if (database_reconnect()) {
            status = database_connection->Put(leveldb::WriteOptions(), key, value);

            if (!status.ok()) {
                logger->log("database_put", ("key: " + key + " value: " + value + " put failed then reconnect but put failed again...").c_str());
                return false;
            } else {
                return true;
            }
        } else {
                logger->log("database_put", ("key: " + key + " value: " + value + " put failed then reconnect failed...").c_str());
                return false;
        }
    }
}

bool KyrinDatabaseWrapper::get(const string &key, string &value)
{
    leveldb::Status status = database_connection->Get(leveldb::ReadOptions(), key, &value);
    if (kyrin_likely(status.ok())) {
        return true;
    } else if (status.IsNotFound()) {
        value = "";
        return false;
    } else {
        if (database_reconnect()) {
            status = database_connection->Get(leveldb::ReadOptions(), key, &value);

            if (!status.ok()) {
                logger->log("database_get", ("key: " + key + " get failed then reconnect but get failed again..." + status.ToString()).c_str());
                return false;
            } else {
                return true;
            }
        } else {
                logger->log("database_get", ("key: " + key + " get failed then reconnect failed...").c_str());
                return false;
        }
    }
}

bool KyrinDatabaseWrapper::exist(const string &key)
{
    string value;
    return get(key, value);
}

bool KyrinDatabaseWrapper::remove(const string &key)
{
    leveldb::Status status = database_connection->Delete(leveldb::WriteOptions(), key);
    if (kyrin_likely(status.ok())) {
        return true;
    }
    return false;
}

bool KyrinDatabaseWrapper::fetch_last_key(string &key)
{
    leveldb::Iterator* it = database_connection->NewIterator(leveldb::ReadOptions());
    if (it == NULL) {
        return false;
    }

    it->SeekToLast();
    if (!it->Valid()) {
        return false;
    }

    key.assign(it->key().ToString());

    assert(it->status().ok());
    delete it;
    return true;
}

} /* io */
} /* kyrin */
