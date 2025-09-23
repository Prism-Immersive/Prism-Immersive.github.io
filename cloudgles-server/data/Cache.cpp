
#define LOG_TAG "Cache"

#include "Cache.h"

#include <sys/stat.h>

#include "DataTracer.h"
#include "Log.h"
#include "leveldb/options.h"

Cache::~Cache() { delete db; }

void Cache::get(leveldb::ReadOptions &options, std::string &key,
                std::string *data) {
    leveldb::Status status = db->Get(options, key, data);
    if (!status.ok()) {
        ALOGE("get %s failed: %s", key.c_str(), status.ToString().c_str());
    }
}

void Cache::put(leveldb::WriteOptions &options, std::string &key,
                const char *data, uint64_t len) {
    leveldb::Slice slice(data, len);
    leveldb::Status status = db->Put(options, key, slice);
    if (!status.ok()) {
        ALOGE("put %s failed: %s", key.c_str(), status.ToString().c_str());
    }
}

void Cache::remove(leveldb::WriteOptions &options, std::string &key) {
    leveldb::Status status = db->Delete(options, key);
    if (!status.ok()) {
        ALOGE("remove %s failed: %s", key.c_str(), status.ToString().c_str());
    }
}

void Cache::init_leveldb(leveldb::Options &options,
                         const std::string &file_name) {
    std::string package_name = DataTracer::get_app_package_name();
    std::string dir_path =
        std::string(BASE_DIR) + package_name;
    if (access(dir_path.c_str(), F_OK) == -1 &&
        mkdir(dir_path.c_str(), 0777) == -1) {
        if (package_name.find(":") != std::string::npos) {
            package_name = package_name.substr(0, package_name.find(":"));
            dir_path = std::string(BASE_DIR) + package_name;
            if (access(dir_path.c_str(), F_OK) == -1 &&
                mkdir(dir_path.c_str(), 0777) == -1) {
                ALOGE("mkdir %s failed", dir_path.c_str());
            }
        } else {
            ALOGE("mkdir %s failed", dir_path.c_str());
        }
    }

    leveldb_path = dir_path + "/" + file_name;

    options.create_if_missing = true;
    leveldb::DB::Open(options, leveldb_path, &db);
}