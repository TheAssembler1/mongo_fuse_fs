#pragma once

#include <mongocxx/client.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <vector>
#include <cassert>
#include <optional>
#include <optional>

#include "types.h"
#include "fs_lookup_collection.h"
#include "fs_metadata_collection.h"
#include "../connection.h"
#include "../manager.h"

using bsoncxx::document::value;

namespace mongo {
  class FSDataCollectionEntry {
    public:
      FSDataCollectionEntry(FS_DATA_ID fs_data_id, long int size, char* buf) : fs_data_id{fs_data_id}, size{size}, buf{buf} {}
      FSDataCollectionEntry(long int size, char* buf) : size{size}, buf{buf} {}

      static FSDataCollectionEntry bson_to_entry(value bson_doc);
    
      static constexpr std::string_view FS_DATA_ID_KEY = "_id";
      static constexpr std::string_view FS_BUF_KEY = "buf";
      static constexpr std::string_view FS_BUF_SIZE_KEY = "size";
      
      std::optional<FS_DATA_ID> fs_data_id = std::nullopt;
      const long int size;
      const char* buf;
  };


  class FSDataCollection {
    public:
      static std::optional<int> create_entry(INODE inode, FSDataCollectionEntry fs_data_collection_entry);
      static std::optional<FSDataCollectionEntry> read_entry(FS_DATA_ID fs_data_id);
      static std::vector<FSDataCollectionEntry> read_all_fs_data_blocks(INODE inode);

      static constexpr std::string_view NAME = "fs_data";
  };
}
