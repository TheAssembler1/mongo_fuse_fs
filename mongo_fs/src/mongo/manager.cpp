#include "manager.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::array;
using bsoncxx::builder::basic::document;

using namespace mongo;

void Manager::init_db() {
  std::cout << "init database" << std::endl;

  struct stat root_fs_dir_stat = fs::Helper::get_stat_of_root_fs(); 
  fs::Mode mode{root_fs_dir_stat.st_mode};
  FSMetadataCollectionEntry fs_metadata_collection_entry {
    (INODE)root_fs_dir_stat.st_ino, 
    Manager::fs_root_path, 
    std::string{FSMetadataCollectionEntry::FILE_TYPE_DIR_VALUE},
    mode, 
    root_fs_dir_stat.st_atim.tv_sec,
    root_fs_dir_stat.st_mtim.tv_sec,
    root_fs_dir_stat.st_ctim.tv_sec,
    (int)root_fs_dir_stat.st_uid,
    (int)root_fs_dir_stat.st_gid,
  }; 

  FSMetadataCollection::create_entry(fs_metadata_collection_entry);
}

int Manager::generate_id() {
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<int> dis;
  
  return dis(gen);
}
