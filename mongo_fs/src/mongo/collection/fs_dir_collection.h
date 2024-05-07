#pragma once

// NOTE: needed to establish fuse version early
#define FUSE_USE_VERSION 35
#include <fuse.h>
#include <cassert>
#include <random>
#include <iostream>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <algorithm>
#include <iostream>
#include <vector>

#include "../../fs/perm.h"
#include "../manager.h"
#include "../connection.h"
#include "fs_lookup_collection.h"
#include "fs_data_collection.h"

using bsoncxx::v_noabi::document::value;
using bsoncxx::v_noabi::document::view;
using bsoncxx::v_noabi::document::view_or_value;

namespace mongo {

  class FSDirCollectionEntry {
    public:
      FSDirCollectionEntry(INODE dir_inode, INODE parent_dir_inode, INODE file_inode) : dir_inode{dir_inode}, parent_dir_inode{parent_dir_inode}, file_inode{file_inode} {}
    
      const value to_document(); 
      static FSDirCollectionEntry bson_to_dir_collection_entry(view bson_doc);

      static constexpr std::string_view DIR_INODE_KEY = "_id";
      static constexpr std::string_view PARENT_DIR_INODE_KEY = "parent_dir_inode";
      static constexpr std::string_view FILE_INODE_KEY = "file_inode";
    
      INODE dir_inode;
      INODE parent_dir_inode;
      INODE file_inode;
  };

  class FSDirCollection {
    public:
      static std::optional<INODE> create_entry(INODE dir_inode, INODE parent_dir_inode, INODE file_inode);
      static std::vector<INODE> read_file_inodes(INODE dir_inode);

      static constexpr std::string_view NAME = "fs_dir";
  };

}
