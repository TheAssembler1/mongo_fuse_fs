#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>

namespace fs {

  class Helper {
    public:
      static std::vector<std::string> get_path_components(const char* path);
  };

}
