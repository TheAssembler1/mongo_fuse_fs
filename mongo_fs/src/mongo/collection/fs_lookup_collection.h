#ifndef __FS_LOOKUP_COLLECTION_H__
#define __FS_LOOKUP_COLLECTION_H__

#include <algorithm>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <iostream>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <vector>

#include "../connection.h"
#include "fs_data_collection.h"
#include "fs_metadata_collection.h"
#include "types.h"

using bsoncxx::binary_sub_type;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::types::b_binary;
using bsoncxx::v_noabi::document::value;
using bsoncxx::v_noabi::document::view;
using bsoncxx::v_noabi::document::view_or_value;

namespace mongo {

class FSLookupCollectionEntry {
    public:
    FSLookupCollectionEntry(INODE inode, FS_DATA_ID fs_data_id) : inode{inode}, fs_data_id{fs_data_id} {
    }
    FSLookupCollectionEntry(INODE inode, int order, FS_DATA_ID fs_data_id)
    : inode{inode}, order{order}, fs_data_id{fs_data_id} {
    }

    static FSLookupCollectionEntry bson_to_entry(value bson_doc);

    static constexpr std::string_view INODE_KEY         = "inode";
    static constexpr std::string_view ORDER_KEY         = "order";
    static constexpr std::string_view DATA_BLOCK_ID_KEY = "data_block_id";

    std::optional<int> fs_lookup_id;
    INODE inode;
    int order;
    FS_DATA_ID fs_data_id;
};

class FSLookupCollection {
    public:
    static std::optional<int> get_max_order(INODE inode);
    static std::optional<int> create_next_entry(FSLookupCollectionEntry& fs_lookup_collection_entry);
    static std::optional<FSLookupCollectionEntry> read_entry_with_inode_order(INODE inode, int order);
    static std::vector<FS_DATA_ID> get_fs_data_ids(INODE inode);

    static constexpr std::string_view NAME = "fs_lookup";
};

} // namespace mongo

#endif
