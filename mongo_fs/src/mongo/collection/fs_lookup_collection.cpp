#include <cassert>

#include "fs_lookup_collection.h"

using namespace mongo;

#define MONGO_ASC_ORDER 1
#define MONGO_DESC_ORDER -1

FSLookupCollectionEntry FSLookupCollectionEntry::bson_to_entry(value bson_doc) {
    return FSLookupCollectionEntry(
    bson_doc[INODE_KEY].get_int32(), bson_doc[ORDER_KEY].get_int32(), bson_doc[DATA_BLOCK_ID_KEY].get_int32());
}

std::optional<int> FSLookupCollection::get_max_order(INODE inode) {
    Connection conn;

    auto fs_blocks_collection = GET_FS_LOOKUP_COLLECTION(&conn);

    auto query_pipeline = mongocxx::pipeline();
    query_pipeline.match(make_document(kvp(FSLookupCollectionEntry::INODE_KEY, inode)));
    query_pipeline.sort(make_document(kvp(FSLookupCollectionEntry::ORDER_KEY, MONGO_DESC_ORDER)));
    query_pipeline.limit(1);

    auto cursor                   = fs_blocks_collection.aggregate(query_pipeline);
    auto prev_max_fs_lookup_block = *(cursor.begin());

    if(prev_max_fs_lookup_block.empty()) {
        return std::nullopt;
    }

    return prev_max_fs_lookup_block[FSLookupCollectionEntry::ORDER_KEY].get_int32() + 1;
}

std::optional<int> FSLookupCollection::create_next_entry(FSLookupCollectionEntry& fs_lookup_collection_entry) {
    Connection conn;
    auto fs_lookup_collection = GET_FS_LOOKUP_COLLECTION(&conn);
    auto last_order           = get_max_order(fs_lookup_collection_entry.inode);

    FS_DATA_ID cur_order = 0;
    if(last_order.has_value()) {
        cur_order = last_order.value();
    }

    auto query_res =
    fs_lookup_collection.insert_one(make_document(kvp(FSLookupCollectionEntry::INODE_KEY, fs_lookup_collection_entry.inode),
    kvp(FSLookupCollectionEntry::DATA_BLOCK_ID_KEY, fs_lookup_collection_entry.fs_data_id),
    kvp(FSLookupCollectionEntry::ORDER_KEY, cur_order)));

    assert(query_res);

    if(!query_res.has_value()) {
        return std::nullopt;
    }

    return fs_lookup_collection_entry.inode;
}

std::optional<FSLookupCollectionEntry> FSLookupCollection::read_entry_with_inode_order(INODE inode, int order) {
    std::optional<FSLookupCollectionEntry> res = std::nullopt;

    Connection conn;
    auto collection = GET_FS_LOOKUP_COLLECTION(&conn);

    auto doc_bson = collection.find_one(
    make_document(kvp(FSLookupCollectionEntry::INODE_KEY, inode), kvp(FSLookupCollectionEntry::ORDER_KEY, order)));

    assert(doc_bson);

    return FSLookupCollectionEntry::bson_to_entry(doc_bson.value());
}

std::vector<FS_DATA_ID> FSLookupCollection::get_fs_data_ids(INODE inode) {
    Connection conn;

    auto lookup_collection = GET_FS_LOOKUP_COLLECTION(&conn);

    auto query_pipeline = mongocxx::pipeline();
    query_pipeline.match(make_document(kvp(FSLookupCollectionEntry::INODE_KEY, inode)));
    query_pipeline.sort(make_document(kvp(FSLookupCollectionEntry::ORDER_KEY, MONGO_ASC_ORDER)));
    auto cursor = lookup_collection.aggregate(query_pipeline);

    std::vector<FS_DATA_ID> result_vec;

    for(auto& item : cursor) {
        auto fs_id = item[FSLookupCollectionEntry::DATA_BLOCK_ID_KEY].get_int32();
        result_vec.push_back(fs_id);
    }

    return result_vec;
}

std::vector<FS_DATA_ID> FSLookupCollection::remove_above_order(INODE inode, int above_order) {
   Connection conn;
   auto collection = GET_FS_LOOKUP_COLLECTION(&conn);

   std::cout << "querying for lookup docs with inode: " << inode << std::endl;
   std::cout << "querying for lookup docs with order $gte: " << above_order << std::endl;

   auto query = make_document(
            kvp(FSLookupCollectionEntry::INODE_KEY, inode),
            kvp(FSLookupCollectionEntry::ORDER_KEY, make_document(
                  kvp("$gte", above_order)
            ))
         );

   auto res_ids_bson = collection.find(query.view());

   std::vector<FS_DATA_ID> fs_data_ids{};

   for(const auto& entry: res_ids_bson) {
      auto id = entry[FSLookupCollectionEntry::DATA_BLOCK_ID_KEY].get_int32();
      fs_data_ids.push_back(id);
      std::cout << "removing data block with id: " << id << std::endl;
   }

   auto res = collection.delete_many(query.view());

   return fs_data_ids;
}
