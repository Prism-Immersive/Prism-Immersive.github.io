#ifndef CACHE_H
#define CACHE_H

#include <leveldb/db.h>
#include <stdint.h>

#include <string>

#include "leveldb/options.h"

class Cache {
   public:
    virtual ~Cache();

   protected:
    void get(leveldb::ReadOptions &options, std::string &key,
             std::string *data);
    void put(leveldb::WriteOptions &options, std::string &key, const char *data,
             uint64_t len);
    void remove(leveldb::WriteOptions &options, std::string &key);

    void init_leveldb(leveldb::Options &options, const std::string &file_name);

    leveldb::DB *get_db() { return db; }

   private:
    std::string leveldb_path;
    leveldb::DB *db;
};

#endif  // CACHE_H