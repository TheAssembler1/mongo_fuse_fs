#include <cassert>

#include "fs_lookup.h"
#include "connection.h"

using namespace mongo;

#define MONGO_ASC_ORDER 1
#define MONGO_DESC_ORDER -1

std::optional<int> FSLookup::get_max_order() {
  Connection conn; 

  auto fs_blocks_collection = GET_FS_BLOCKS_COLLECTION(&conn);

  auto query_pipeline = mongocxx::pipeline();
  query_pipeline.sort(make_document(kvp(FS_ID_KEY, MONGO_DESC_ORDER)));
  query_pipeline.limit(1);

  auto cursor = fs_blocks_collection.aggregate(query_pipeline);
  auto prev_max_fs_lookup_block = *(cursor.begin());

  if(prev_max_fs_lookup_block.empty()) {
    return std::nullopt;
  }

  return prev_max_fs_lookup_block[FS_ID_KEY].get_int64() + 1; 
}

std::optional<int> FSLookup::create_entry(FS_ID fs_id) {
  Connection conn; 
  auto fs_blocks_collection = GET_FS_BLOCKS_COLLECTION(&conn);
  auto last_order = get_max_order();

  FS_ID cur_order = 0;
  if(last_order.has_value()) {
    cur_order = last_order.value();
  }
  
  auto query_res = fs_blocks_collection.insert_one(make_document(
    kvp(FS_LOOKUP_ID_KEY, fd),
    kvp(FS_LOOKUP_FS_ENTRY_KEY,  fs_id),
    kvp(FS_LOOKUP_FILE_ORDER_NUMBER_KEY, cur_order)
  ));

  assert(query_res);

  if(!query_res.has_value()) {
    return std::nullopt;
  }

  return fd;
}

std::vector<FS_ID> FSLookup::get_ordered_fs_ids() {
  Connection conn; 

  auto fs_blocks_collection = GET_FS_BLOCKS_COLLECTION(&conn);

  auto query_pipeline = mongocxx::pipeline();
  query_pipeline.sort(make_document(kvp(FS_ID_KEY, MONGO_ASC_ORDER)));
  auto cursor = fs_blocks_collection.aggregate(query_pipeline);
  
  std::vector<INODE> result_vec;

  for(auto& item: cursor) {
    auto fs_id = item[FS_ID_KEY].get_int32(); 
  }

  return result_vec;
}
