#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
#include "../mongo/manager.h"
#include "../mongo/collection/types.h"

namespace fs {

  class Helper {
    public:
      static std::vector<std::string> get_path_components(const char* path);
      static struct stat get_stat_of_root_fs();
      static INODE get_inode_of_root_fs();
  };

}
