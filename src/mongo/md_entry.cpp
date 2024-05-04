// NOTE: needed to establish fuse version early
#define FUSE_USE_VERSION 35
#include <fuse.h>
#include <cassert>
#include <random>
#include <iostream>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <algorithm>
#include <iostream>
#include <vector>


#include "md_entry.h"
#include "manager.h"
#include "connection.h"
#include "md_entry.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

using namespace mongo;

MDEntry::MDEntry(const char* _path, mode_t _mode, MDFileType file_type) {
  std::cout << "creating file md for " << path << std::endl;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<long> dis;
  fd = dis(gen);
  std::cout << "generated random fd " << fd << std::endl;

  path = std::string(_path);
  md_type = (file_type == MDFileType::DIR) ? MD_FS_FILE_MD_TYPE : MD_FS_DIR_MD_TYPE;
  mode = fs::Mode{_mode};

  auto fuse_context = fuse_get_context();
  auto cur_time = time(NULL);
  last_access = cur_time;
  last_change = cur_time;
  last_modify = cur_time;

  // NOTE: uid/gid unable to be converted to proper type for mongodb
  uid = (int)fuse_context->uid;
  gid = (int)fuse_context->gid;
}

const INODE MDEntry::create_entry() {
  Connection conn;
  auto md_collection = GET_MD_COLLECTION(&conn);

  auto root_dir_doc = md_collection.insert_one(make_document(
      kvp(INODE_KEY, fd),
      kvp(MD_DOC_PATH_KEY, path),

      // md type
      kvp(MD_TYPE_KEY, MD_FS_FILE_MD_TYPE),

      // user perms
      kvp(USER_PERM_READ_KEY, mode.user_perm.read),
      kvp(USER_PERM_WRITE_KEY, mode.user_perm.write),
      kvp(USER_PERM_EXEC_KEY, mode.user_perm.exec),
      
      // group perms
      kvp(GROUP_PERM_READ_KEY, mode.user_perm.read),
      kvp(GROUP_PERM_WRITE_KEY, mode.user_perm.write),
      kvp(GROUP_PERM_EXEC_KEY, mode.user_perm.exec),

      // univ perms
      kvp(UNIV_PERM_EXEC_KEY, mode.user_perm.read),
      kvp(UNIV_PERM_WRITE_KEY, mode.user_perm.write),
      kvp(UNIV_PERM_EXEC_KEY, mode.user_perm.exec),

      // set timestamps
      kvp(LAST_ACCESS_KEY, last_access),
      kvp(LAST_MODIFY_KEY, last_modify),
      kvp(LAST_CHANGE_KEY, last_change),

      // set id's
      kvp(UID_KEY, uid),
      kvp(GID_KEY, gid)
  ));
  
  assert(root_dir_doc);

  return fd;
}

std::optional< MDEntry> MDEntry::search_by_fd(INODE fd) {
  Connection conn;
  auto md_collection = GET_MD_COLLECTION(&conn);
  auto doc = md_collection.find_one(make_document(kvp(INODE_KEY, fd)));

  if(!doc.has_value()) {
    std::cout << "md entry not found fd: " << fd << std::endl;

    return std::nullopt;
  }

  return bson_to_md_entry(doc.value());
}

std::optional<MDEntry> MDEntry::search_by_path(std::string path) {
  Connection conn;
  auto md_collection = GET_MD_COLLECTION(&conn);
  auto doc = md_collection.find_one(make_document(kvp(MD_DOC_PATH_KEY, path)));
  
  if(!doc.has_value()) {
    std::cout << "md entry not found path: " << path << std::endl;

    return std::nullopt;
  }

  return bson_to_md_entry(doc.value());
}

const mode_t MDEntry::to_mode_t() {
  mode_t res = 0;
}

MDEntry MDEntry::bson_to_md_entry(value bson_doc) {
  fs::Mode mode(
  fs::Perm{bson_doc[USER_PERM_READ_KEY].get_bool().value, 
           bson_doc[USER_PERM_WRITE_KEY].get_bool().value, 
           bson_doc[USER_PERM_EXEC_KEY].get_bool().value}, 
  fs::Perm{
           bson_doc[GROUP_PERM_READ_KEY].get_bool().value, 
           bson_doc[GROUP_PERM_WRITE_KEY].get_bool().value, 
           bson_doc[GROUP_PERM_EXEC_KEY].get_bool().value}, 
  fs::Perm{bson_doc[UNIV_PERM_READ_KEY].get_bool().value, 
           bson_doc[UNIV_PERM_WRITE_KEY].get_bool().value, 
           bson_doc[UNIV_PERM_EXEC_KEY].get_bool().value}
  );

  return MDEntry(
    bson_doc[INODE_KEY].get_int64().value, 
    std::string(bson_doc[MD_DOC_PATH_KEY].get_string().value), 
    std::string(bson_doc[MD_FS_DIR_MD_TYPE].get_string().value),
    mode,
    bson_doc[LAST_ACCESS_KEY].get_timestamp().timestamp,
    bson_doc[LAST_MODIFY_KEY].get_timestamp().timestamp,
    bson_doc[LAST_CHANGE_KEY].get_timestamp().timestamp,
    bson_doc[UID_KEY].get_int64().value,
    bson_doc[GID_KEY].get_int64().value);
}