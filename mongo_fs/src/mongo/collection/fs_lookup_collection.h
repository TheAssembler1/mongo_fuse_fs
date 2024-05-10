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

using bsoncxx::types::b_binary;
using bsoncxx::binary_sub_type;
using bsoncxx::binary_sub_type;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::v_noabi::document::value;
using bsoncxx::v_noabi::document::view;
using bsoncxx::v_noabi::document::view_or_value;

namespace mongo {

  class FSLookupCollectionEntry {
    public:
      FSLookupCollectionEntry(INODE inode, FS_DATA_ID fs_data_id) : inode{inode}, fs_data_id {fs_data_id} {}
      FSLookupCollectionEntry(INODE inode, int order, FS_DATA_ID fs_data_id) : inode{inode}, order{order}, fs_data_id {fs_data_id} {}

      static FSLookupCollectionEntry bson_to_entry(value bson_doc);

      static constexpr std::string_view ID_INODE_KEY = "_id";
      static constexpr std::string_view ORDER_KEY = "order";
      static constexpr std::string_view DATA_BLOCK_ID_KEY = "data_block_id";
      
      std::optional<int> fs_lookup_id;
      INODE inode;
      int order;
      FS_DATA_ID fs_data_id;
  };

  class FSLookupCollection {
    public:
      static std::optional<int> get_max_order();
      static std::optional<int> create_entry(FSLookupCollectionEntry& fs_lookup_collection_entry);
      static std::optional<FSLookupCollectionEntry> read_entry_with_inode_order(INODE inode, int order);

      static constexpr std::string_view NAME = "fs_lookup";
  };

}
