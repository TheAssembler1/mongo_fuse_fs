#include "fs_data_collection.h"

using namespace mongo;

using bsoncxx::types::b_binary;
using bsoncxx::binary_sub_type;
using bsoncxx::binary_sub_type;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

std::optional<int> FSDataCollection::create_entry(INODE inode, FSDataCollectionEntry fs_data_collection_entry) {
  Connection conn;
  auto file_collection = GET_FS_DATA_COLLECTION(&conn);

  b_binary data{binary_sub_type::k_binary, static_cast<std::uint32_t>(fs_data_collection_entry.size), reinterpret_cast<const std::uint8_t*>(fs_data_collection_entry.buf)};
  
  std::cout << "creating entry for fs_data_collection" << std::endl;
  int fs_data_id = Manager::generate_id();
  auto result_doc =  file_collection.insert_one(make_document(
    kvp(FSDataCollectionEntry::FS_DATA_ID_KEY, fs_data_id),
    kvp(FSDataCollectionEntry::FS_BUF_KEY, data),
    kvp(FSDataCollectionEntry::FS_BUF_SIZE_KEY, fs_data_collection_entry.size)
  ));

  assert(result_doc);

  if(!result_doc.has_value()) {
    return std::nullopt;
  }

  std::cout << "creating entry for fs_lookup_collection" << std::endl;
  FSLookupCollection fs_lookup_collection_entry{inode};
  fs_lookup_collection_entry.create_entry(fs_data_id);

  return fs_data_collection_entry.size;
}

FSDataCollectionEntry FSDataCollectionEntry::bson_to_entry(value bson_doc) {
  return FSDataCollectionEntry {
    bson_doc[FSDataCollectionEntry::FS_DATA_ID_KEY].get_int32().value,
    bson_doc[FSDataCollectionEntry::FS_BUF_SIZE_KEY].get_int32().value,
    (char*)bson_doc[FSDataCollectionEntry::FS_BUF_KEY].get_binary().bytes
  };
}

std::optional<FSDataCollectionEntry> FSDataCollection::read_entry(FS_DATA_ID fs_id) {
  Connection conn;
  auto file_collection = GET_FS_DATA_COLLECTION(&conn);

  auto bson_res = file_collection.find_one(make_document(kvp(FSDataCollectionEntry::FS_DATA_ID_KEY, fs_id)));

  assert(bson_res);

  if(!bson_res.has_value()) {
    return std::nullopt;
  }

  auto bson_doc = bson_res.value();
  return FSDataCollectionEntry::bson_to_entry(bson_doc);
}


std::vector<FSDataCollectionEntry> FSDataCollection::read_all_fs_data_blocks(INODE inode) {
  FSLookupCollection fs_lookup(inode); 
  std::vector<FS_DATA_ID> fs_data_ids = fs_lookup.get_fs_data_ids();
  std::vector<FSDataCollectionEntry> fs_data_entries;
  
  // FIXME : assumes only has one block
  for(auto fs_data_id: fs_data_ids) {
    std::optional<FSDataCollectionEntry> fs_data_collection_entry_opt = FSDataCollection::read_entry(fs_data_id);

    assert(fs_data_collection_entry_opt.has_value());
    fs_data_entries.push_back(fs_data_collection_entry_opt.value());
  }

  return fs_data_entries;
}
