#pragma once

// NOTE: needed to establish fuse version early
#define FUSE_USE_VERSION 35
#include <fuse.h>
#include <iostream>
#include <cassert>

#include "../manager.h"
#include "fs_metadata_collection.h"
#include "fs_data_collection.h"
#include "fs_lookup_collection.h"
#include "types.h"

namespace mongo {

  class CollectionHelper {
    public:
      static void create_next_data_lookup_entries(INODE inode);
      static void write_fuse_bufvec_to_mongo(const char* path, fuse_bufvec& f_bvec, fuse_file_info& ffi);
      static void print_all_file(INODE inode);
  };

}

