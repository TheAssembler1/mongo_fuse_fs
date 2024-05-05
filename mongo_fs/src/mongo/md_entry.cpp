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
#include "fs_lookup.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

using namespace mongo;

MDEntry::MDEntry(const char* _path, mode_t _mode, MDFileType file_type) {
  std::cout << "creating file md for " << path << std::endl;

  fd = Manager::generate_id();
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

  std::cout << "md uid: " << uid << std::endl;
  std::cout << "md gid: " << gid << std::endl;
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
      kvp(UNIV_PERM_READ_KEY, mode.user_perm.read),
      kvp(UNIV_PERM_WRITE_KEY, mode.user_perm.write),
      kvp(UNIV_PERM_EXEC_KEY, mode.user_perm.exec),

      // set timestamps
      kvp(LAST_ACCESS_KEY, last_access),
      kvp(LAST_MODIFY_KEY, last_modify),
      kvp(LAST_CHANGE_KEY, last_change),

      // set id's
      kvp(UID_KEY, (int)uid),
      kvp(GID_KEY, (int)gid)
  ));
  
  assert(root_dir_doc);

  return fd;
}

std::optional< MDEntry> MDEntry::search_by_fd(INODE fd) {
  Connection conn;
  auto md_collection = GET_MD_COLLECTION(&conn);
  std::cout << "attempting search for fd: " << fd << std::endl;
  auto doc = md_collection.find_one(make_document(kvp(INODE_KEY, fd)));

  if(!doc.has_value()) {
    std::cout << "md entry not found fd: " << fd << std::endl;

    return std::nullopt;
  } else {
    std::cout << "doc found with fd: " << fd << std::endl;
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
  } else {
    std::cout << "doc found with path: " << path << std::endl;
  }

  return bson_to_md_entry(doc.value());
}

// https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_stat.h.html
const mode_t MDEntry::to_mode_t() {
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

  if(md_type == MD_FS_FILE_MD_TYPE) { return S_IFREG | res; }

  return S_IFDIR | res;
}

MDEntry MDEntry::bson_to_md_entry(value bson_doc) {
  std::cout << "getting bson permission values" << std::endl;
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

  std::cout << "getting bson timestamps values" << std::endl;
  time_t last_access = bson_doc[LAST_ACCESS_KEY].get_int64().value;
  time_t last_modify = bson_doc[LAST_MODIFY_KEY].get_int64().value;
  time_t last_change = bson_doc[LAST_CHANGE_KEY].get_int64().value;
  
  std::cout << "getting inode value" << std::endl;
  INODE fd = bson_doc[INODE_KEY].get_int32().value;
  
  std::cout << "getting uid value" << std::endl; 
  int uid = bson_doc[UID_KEY].get_int32().value;

  std::cout << "getting gid value" << std::endl;
  int gid = bson_doc[GID_KEY].get_int32().value;

  std::cout << "getting other mode values" << std::endl;
  return MDEntry(
    fd, 
    std::string(bson_doc[MD_DOC_PATH_KEY].get_string().value), 
    std::string(bson_doc[MD_TYPE_KEY].get_string().value),
    mode,
    time(&last_access),
    time(&last_modify),
    time(&last_change),
    uid,
    gid
  );
}

void MDEntry::read_all_data_blocks(char** _buf) {
  FSLookup fs_lookup(fd); 
  std::vector<FS_ID> fs_ids = fs_lookup.get_ordered_fs_ids();
  
  // FIXME : assumes only has one block
  for(auto fs_id: fs_ids) {
    std::optional<char*> buf_opt = FSEntry::read_entry_data(fs_id);

    assert(buf_opt.has_value());
    char* buf = buf_opt.value();
      
    *_buf = buf;
  }
}
