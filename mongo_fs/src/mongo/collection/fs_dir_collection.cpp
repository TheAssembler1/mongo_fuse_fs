#include "fs_dir_collection.h"

using namespace mongo;


const value FSDirCollectionEntry::to_document() {
  return make_document(
    kvp(DIR_INODE_KEY, dir_inode),
    kvp(FILE_INODE_KEY, file_inode)
  );
}

FSDirCollectionEntry FSDirCollectionEntry::bson_to_dir_collection_entry(view bson_doc) {
  return FSDirCollectionEntry {
    bson_doc[DIR_INODE_KEY].get_int32().value,
    bson_doc[FILE_INODE_KEY].get_int32().value
  };
}

std::optional<INODE> FSDirCollection::create_entry(INODE dir_inode, INODE file_inode) {
  Connection conn;
  auto fs_dir_collection = GET_FS_DIR_COLLECTION(&conn);

  FSDirCollectionEntry fs_dir_collection_entry{dir_inode, file_inode};
  auto doc_res_bson = fs_dir_collection.insert_one(fs_dir_collection_entry.to_document().view());
  assert(doc_res_bson);

  return dir_inode;
}

std::vector<INODE> FSDirCollection::read_file_inodes(INODE dir_inode) {
  std::vector<INODE> res_inodes;

  Connection conn;
  auto fs_dir_collection = GET_FS_DIR_COLLECTION(&conn);

  auto docs_res_bson = fs_dir_collection.find(make_document(kvp(FSDirCollectionEntry::DIR_INODE_KEY, dir_inode)));
  
  for(auto doc_res_bson: docs_res_bson) {
    FSDirCollectionEntry fs_dir_collection_entry = FSDirCollectionEntry::bson_to_dir_collection_entry(doc_res_bson);
    res_inodes.push_back(fs_dir_collection_entry.file_inode);
  }

  return res_inodes;
}
