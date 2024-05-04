#pragma once

#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <sys/stat.h>

#include "connection.h"

// NOTE: for database =========================================
// the name of the database used to store all fs data
#define ROOT_DB_NAME "fs_root"
#define INODE_KEY "_id"

// NOTE: for file entries =====================================
// holds blocks of data for files
#define FILE_COLLECTION_NAME "fs_file"

#define BLOCK_SIZE 512

namespace mongo {

  class Manager {
    public:
      static void init_db();

      static inline uint64_t cur_max_fd = 0;
      static const inline mongocxx::instance inst;
  };


}
