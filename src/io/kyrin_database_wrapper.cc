#include "kyrin_database_wrapper.h"
#include "common/kyrin_log.h"

namespace kyrin {
namespace io {

using namespace std;
using namespace kyrin::common;

static KyrinLog *logger = KyrinLog::get_instance(); 
KyrinDatabaseWrapper::KyrinDatabaseWrapper()
{
    database_connect();
}

KyrinDatabaseWrapper::~KyrinDatabaseWrapper()
{
    database_disconnect();
}

bool KyrinDatabaseWrapper::database_connect()
{
    database_options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(database_options, "/tmp/kyrindb", &database_connection);
    if (/* unlikely */!status.ok()) {
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

bool KyrinDatabaseWrapper::database_put(const string &key, const string &value)
{
    leveldb::Status status = database_connection->Put(leveldb::WriteOptions(), key, value);
    if (/* likely */status.ok()) {
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

bool KyrinDatabaseWrapper::database_get(const string &key, string &value)
{
    leveldb::Status status = database_connection->Get(leveldb::ReadOptions(), key, &value);
    if (/* likely */status.ok()) {
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
    
} /* io */
} /* kyrin */
