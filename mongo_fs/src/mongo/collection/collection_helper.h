#pragma once

// NOTE: needed to establish fuse version early
#define FUSE_USE_VERSION 35
#include <fuse.h>
#include <iostream>
#include <cassert>

#include "fs_metadata_collection.h"
#include "types.h"

namespace mongo {

  class CollectionHelper {
    public:
      static void write_blocks_to_mongo(const char* path, fuse_bufvec& f_bvec, fuse_file_info& ffi);
  };

}

