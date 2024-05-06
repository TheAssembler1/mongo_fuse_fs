#include "helper.h"

using namespace fs;

std::vector<std::string> Helper::get_path_components(const char* c_str_path) {
  std::vector<std::string> res_vec{};
  std::cout << "parsing path: " << c_str_path << " into components" << std::endl;
  std::filesystem::path path{c_str_path};

  if(!path.has_parent_path()) {
    std::cout << "path had not parent directory" << std::endl;
    res_vec.push_back(path.string());
  }

  while(path.has_parent_path()) {
    if(path == "/") {
      res_vec.push_back(std::string{"/"});
      break;
    }

    std::filesystem::path base = path.filename();
    res_vec.push_back(base.string());
    path = path.parent_path();
  }

  // FIXME: do better?
  std::reverse(res_vec.begin(), res_vec.end());
  
  std::cout << "result path" << std::endl << std::endl;
  for(std::string str: res_vec) {
    std::cout << str << std::endl;
  }
  std::cout << std::endl << "end result path" << std::endl;

  return res_vec;
}
