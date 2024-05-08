#pragma once

// NOTE: needed to establish fuse version early
#define FUSE_USE_VERSION 35
#include <fuse.h>
#include <iostream>
#include <assert.h>
#include <random>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <sys/stat.h>
#include <time.h>

#include "collection/fs_metadata_collection.h"
#include "connection.h"
#include "../fs/perm.h"
#include "../fs/fs_helper.h"

// NOTE: for database =========================================
// the name of the database used to store all fs data
#define ROOT_DB_NAME "fs_root"
#define BLOCK_SIZE 512
// FIXME: put somewhere better?
#define MAX_FS_ROOT_PATH 256
#define PARENT_DIR_INODE_OF_ROOT_FS_DIR 0

namespace mongo {

  class Manager {
    public:
      static void init_db();
      static int generate_id();
      static bool parent_dir_is_root(const char* path);

      static inline uint64_t cur_max_fd = 0;
      static inline std::string fs_root_path = std::string{};
      static const inline mongocxx::instance inst;
  };


}
