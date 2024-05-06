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

#include "connection.h"
#include "../fs/perm.h"

// NOTE: for database =========================================
// the name of the database used to store all fs data
#define ROOT_DB_NAME "fs_root"

#define BLOCK_SIZE 512

namespace mongo {

  class Manager {
    public:
      static void init_db();
      static int generate_id();

      static inline uint64_t cur_max_fd = 0;
      static const inline mongocxx::instance inst;
  };


}
