#ifndef __MANAGER_H__
#define __MANAGER_H__

// NOTE: needed to establish fuse version early
#define FUSE_USE_VERSION 35
#include <assert.h>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <fuse.h>
#include <iostream>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <random>
#include <sys/stat.h>
#include <time.h>

#include "../fs/fs_helper.h"
#include "../fs/perm.h"
#include "connection.h"

using bsoncxx::builder::basic::array;
using bsoncxx::builder::basic::document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

// NOTE: for database =========================================
// the name of the database used to store all fs data
#define ROOT_DB_NAME "fs_root"
// FIXME: put somewhere better?
#define MAX_FS_ROOT_PATH 256
#define PARENT_DIR_INODE_OF_ROOT_FS_DIR 0

namespace mongo {

class MongoManager {
    public:
    static void init_db();
    static int generate_id();
    static bool parent_dir_is_root(const char* path);

    static inline uint64_t cur_max_fd      = 0;
    static inline std::string fs_root_path = std::string{};
    static const inline mongocxx::instance inst;
};

} // namespace mongo

#endif
