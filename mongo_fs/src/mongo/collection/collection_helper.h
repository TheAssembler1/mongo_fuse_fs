#pragma once

#include <vector>
#include <string>

#include "../../fs/fs_helper.h"
#include "fs_metadata_collection.h"
#include "fs_dir_collection.h"

namespace mongo {

  class CollectionHelper {
    public:
      static bool folders_to_file_exist(const char* path);
  };

}
