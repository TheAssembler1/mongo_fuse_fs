#include "fs_helper.h"

using namespace fs;

std::string FSHelper::get_base_name_of_path(const char* c_str_path) {
  std::filesystem::path path{c_str_path};
  return path.filename().string();
}


std::vector<std::string> FSHelper::get_path_components(const char* c_str_path) {
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

std::string FSHelper::get_parent_dir_string(const char* c_str_path) {
  std::filesystem::path path{c_str_path};

  if(!path.has_parent_path()) {
    std::cerr << "ERROR: child does not have parent path";
    return std::string{};
  }

  return std::string(path.parent_path().c_str());
}

struct stat FSHelper::get_stat_of_root_fs() {
  struct stat res_stat;
  memset(&res_stat, 0, sizeof(struct stat));

  if(stat(mongo::Manager::fs_root_path.c_str(), &res_stat) <= 0) {
    std::cout << "succesfully stat root fs dir" << std::endl;
    return res_stat;
  } else {
    std::cerr << "ERROR: failed to get stat of root fs" << std::endl;
    return res_stat;
  }

  return res_stat;
}

INODE FSHelper::get_inode_of_root_fs() {
  struct stat res_stat = FSHelper::get_stat_of_root_fs();
  return res_stat.st_ino;
}
