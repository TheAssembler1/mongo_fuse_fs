#include "fs_metadata_collection.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::document::view_or_value;

using namespace mongo;

const value FSMetadataCollectionEntry::to_document() {
  return make_document(
      kvp(INODE_KEY, inode),
      kvp(PATH_KEY, path),

      // md type
      kvp(FILE_TYPE_KEY, file_type),

      // user perms
      kvp(USER_MODE_READ_KEY, mode.user_perm.read),
      kvp(USER_MODE_WRITE_KEY, mode.user_perm.write),
      kvp(USER_MODE_EXEC_KEY, mode.user_perm.exec),
      
      // group perms
      kvp(GROUP_MODE_READ_KEY, mode.user_perm.read),
      kvp(GROUP_MODE_WRITE_KEY, mode.user_perm.write),
      kvp(GROUP_MODE_EXEC_KEY, mode.user_perm.exec),

      // univ perms
      kvp(UNIV_MODE_READ_KEY, mode.user_perm.read),
      kvp(UNIV_MODE_WRITE_KEY, mode.user_perm.write),
      kvp(UNIV_MODE_EXEC_KEY, mode.user_perm.exec),

      // set timestamps
      kvp(LAST_ACCESS_KEY, last_access),
      kvp(LAST_MODIFY_KEY, last_modify),
      kvp(LAST_CHANGE_KEY, last_change),

      // set id's
      kvp(UID_KEY, (int)uid),
      kvp(GID_KEY, (int)gid)
  );
}


FSMetadataCollectionEntry::FSMetadataCollectionEntry(const char* _path, mode_t _mode, MDFileType _file_type) {
  inode = Manager::generate_id();

  path = std::string(_path);
  file_type = (_file_type == MDFileType::FILE) ? FSMetadataCollectionEntry::FILE_TYPE_FILE_VALUE : FSMetadataCollectionEntry::FILE_TYPE_DIR_VALUE;
  mode = fs::Mode{_mode};

  auto fuse_context = fuse_get_context();
  auto cur_time = time(NULL);
  last_access = cur_time;
  last_change = cur_time;
  last_modify = cur_time;

  uid = (int)fuse_context->uid;
  gid = (int)fuse_context->gid;
}

INODE FSMetadataCollection::create_entry(FSMetadataCollectionEntry fs_metadata_collection_entry) {
  Connection conn;
  auto md_collection = GET_FS_METADATA_COLLECTION(&conn);
  auto doc = fs_metadata_collection_entry.to_document();
  auto doc_result = md_collection.insert_one(doc.view());
  
  assert(doc_result);

  return fs_metadata_collection_entry.inode;
}

std::optional<FSMetadataCollectionEntry> FSMetadataCollection::search_by_inode(INODE inode) {
  Connection conn;
  auto md_collection = GET_FS_METADATA_COLLECTION(&conn);
  auto doc = md_collection.find_one(make_document(kvp(FSMetadataCollectionEntry::INODE_KEY, inode)));

  if(!doc.has_value()) {
    return std::nullopt;
  }

  return FSMetadataCollectionEntry::bson_to_md_entry(doc.value());
}

std::optional<FSMetadataCollectionEntry> FSMetadataCollection::search_by_path(std::string path) {
  Connection conn;
  auto md_collection = GET_FS_METADATA_COLLECTION(&conn);
  auto doc = md_collection.find_one(make_document(kvp(FSMetadataCollectionEntry::PATH_KEY, path)));
  
  if(!doc.has_value()) {
    return std::nullopt;
  }

  return FSMetadataCollectionEntry::bson_to_md_entry(doc.value());
}

// https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_stat.h.html
const mode_t FSMetadataCollectionEntry::to_mode_t() {
  mode_t res = 0;

  // user bits
  if(mode.user_perm.read) { res = res | S_IRUSR; }
  if(mode.user_perm.write) { res = res | S_IWUSR; }
  if(mode.user_perm.exec) { res = res | S_IXUSR; }

  // group bits
  if(mode.group_perm.read) { res = res | S_IRGRP; }
  if(mode.group_perm.write) { res = res | S_IWGRP; }
  if(mode.group_perm.exec) { res = res | S_IXGRP; }

  // univ bits
  if(mode.univ_perm.read) { res = res | S_IROTH; }
  if(mode.univ_perm.write) { res = res | S_IWOTH; }
  if(mode.univ_perm.exec) { res = res | S_IXOTH; }

  if(file_type == FILE_TYPE_FILE_VALUE) { return S_IFREG | res; }

  return S_IFDIR | res;
}

FSMetadataCollectionEntry FSMetadataCollectionEntry::bson_to_md_entry(value bson_doc) {
  fs::Mode mode(
  fs::Perm{bson_doc[USER_MODE_READ_KEY].get_bool().value, 
           bson_doc[USER_MODE_WRITE_KEY].get_bool().value, 
           bson_doc[USER_MODE_EXEC_KEY].get_bool().value}, 
  fs::Perm{
           bson_doc[GROUP_MODE_READ_KEY].get_bool().value, 
           bson_doc[GROUP_MODE_WRITE_KEY].get_bool().value, 
           bson_doc[GROUP_MODE_EXEC_KEY].get_bool().value}, 
  fs::Perm{bson_doc[UNIV_MODE_READ_KEY].get_bool().value, 
           bson_doc[UNIV_MODE_WRITE_KEY].get_bool().value, 
           bson_doc[UNIV_MODE_EXEC_KEY].get_bool().value}
  );

  time_t last_access = bson_doc[LAST_ACCESS_KEY].get_int64().value;
  time_t last_modify = bson_doc[LAST_MODIFY_KEY].get_int64().value;
  time_t last_change = bson_doc[LAST_CHANGE_KEY].get_int64().value;
  
  INODE fd = bson_doc[INODE_KEY].get_int32().value;
  
  int uid = bson_doc[UID_KEY].get_int32().value;
  int gid = bson_doc[GID_KEY].get_int32().value;

  return FSMetadataCollectionEntry(
    fd, 
    std::string(bson_doc[PATH_KEY].get_string().value), 
    std::string(bson_doc[FILE_TYPE_KEY].get_string().value),
    mode,
    time(&last_access),
    time(&last_modify),
    time(&last_change),
    uid,
    gid
  );
}