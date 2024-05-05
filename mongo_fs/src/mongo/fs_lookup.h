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

#include "md_entry.h"
#include "fs_entry.h"

#define FS_BLOCKS_COLLECTION_NAME "fs_lookup"
// NOTE: this is the fd/inode id
#define FS_LOOKUP_ID_KEY "_id"
#define FS_LOOKUP_FILE_ORDER_NUMBER_KEY "order"
#define FS_LOOKUP_FS_ENTRY_KEY "fs_entry_id"

using bsoncxx::types::b_binary;
using bsoncxx::binary_sub_type;
using bsoncxx::binary_sub_type;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;


namespace mongo {
  
  class FSLookup {
    public:
      FSLookup(INODE fd) : fd{fd} {}
      std::optional<int> get_max_order();
      std::optional<int> create_entry(FS_ID fs_id);
      std::vector<FS_ID> get_ordered_fs_ids();
    private:
      const INODE fd;
  };

}