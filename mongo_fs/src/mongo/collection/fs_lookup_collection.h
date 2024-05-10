#pragma once

#include <iostream>
#include <mongocxx/client.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <algorithm>
#include <iostream>
#include <vector>

#include "fs_metadata_collection.h"
#include "fs_data_collection.h"
#include "types.h"
#include "../connection.h"

#define FS_LOOKUP_ID_KEY "_id"
#define FS_LOOKUP_FILE_ORDER_NUMBER_KEY "order"
#define FS_LOOKUP_FS_ENTRY_KEY "fs_entry_id"

using bsoncxx::types::b_binary;
using bsoncxx::binary_sub_type;
using bsoncxx::binary_sub_type;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace mongo {

  class FSLookupCollectionEntry {
    public:
      FSLookupCollectionEntry(INODE inode, FS_DATA_ID fs_data_id) : inode{inode}, fs_data_id {fs_data_id} {}
      
      std::optional<int> fs_lookup_id;
      INODE inode;
      FS_DATA_ID fs_data_id;
  };

  class FSLookupCollection {
    public:
      static std::optional<int> get_max_order();
      static std::optional<int> create_entry(FSLookupCollectionEntry& fs_lookup_collection_entry);
      static std::vector<FS_DATA_ID> get_fs_data_ids();

      static constexpr std::string_view NAME = "fs_lookup";
  };

}
