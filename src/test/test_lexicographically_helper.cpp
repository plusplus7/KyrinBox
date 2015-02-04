#include "common/kyrin_lexicographically_helper.h"
#include <iostream>
#include "leveldb/db.h"

using namespace std;
using namespace kyrin::common;

int main()
{
    leveldb::Options o;
    o.create_if_missing = true;
    leveldb::DB *conn;
    leveldb::Status s = leveldb::DB::Open(o, "./test_db", &conn);
    if (!s.ok()) {
        cout << "failed" << endl;
        return 0;
    }
    string key, value;
    KyrinLexicographicallyHelper::get_zero(key);
    for (int i=0; i<1000; i++) {
        conn->Put(leveldb::WriteOptions(), key, to_string(i));
        KyrinLexicographicallyHelper::add_one(key);
    }
    leveldb::Iterator* it = conn->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        string b = it->key().ToString();
        string c = it->value().ToString();
        cout << "value: "  << c << endl;
        for (int i=0; i<b.size(); i++) {
            cout << int(b[i]) << " ";
        }
        cout << endl;
    }
    delete it;
}
