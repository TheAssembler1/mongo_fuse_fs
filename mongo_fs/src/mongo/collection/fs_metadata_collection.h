#ifndef __FS_METADATA_COLLECTION_H__
#define __FS_METADATA_COLLECTION_H__

// NOTE: needed to establish fuse version early
#define FUSE_USE_VERSION 35
#include <algorithm>
#include <cassert>
#include <fuse.h>
#include <iostream>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <random>
#include <vector>

#include "../../fs/perm.h"
#include "../connection.h"
#include "../mongo_manager.h"
#include "collection_helper.h"
#include "fs_data_collection.h"
#include "fs_lookup_collection.h"

using bsoncxx::v_noabi::document::value;
using bsoncxx::v_noabi::document::view;
using bsoncxx::v_noabi::document::view_or_value;

namespace mongo {
enum class MDFileType { FILE, DIR };

class FSMetadataCollectionEntry {
    public:
    FSMetadataCollectionEntry(INODE inode,
    std::string base_name,
    std::string file_type,
    fs::Mode mode,
    time_t last_access,
    time_t last_modify,
    time_t last_change,
    int uid,
    int gid,
    std::optional<int> file_size)
    : inode{inode}, base_name{base_name}, file_type{file_type}, mode{mode}, last_access{last_access},
      last_modify{last_modify}, last_change{last_change}, uid{uid}, gid{gid}, file_size{file_size} {
    }
    FSMetadataCollectionEntry(const char* _base_name, mode_t _mode, MDFileType file_type);

    const mode_t to_mode_t();
    const value to_document();
    const INODE get_parent_dir_inode() {
        return parent_dir_inode;
    }
    void set_parent_dir_inode(INODE _parent_dir_inode) {
        parent_dir_inode = _parent_dir_inode;
    }

    static FSMetadataCollectionEntry bson_to_md_entry(view bson_doc);

    static constexpr std::string_view INODE_KEY            = "_id";
    static constexpr std::string_view PARENT_DIR_INODE_KEY = "parent_dir_inode";
    static constexpr std::string_view BASE_NAME_KEY        = "base_name";
    static constexpr std::string_view FILE_TYPE_KEY        = "file_type";
    static constexpr std::string_view FILE_TYPE_FILE_VALUE = "file";
    static constexpr std::string_view FILE_TYPE_DIR_VALUE  = "dir";
    static constexpr std::string_view USER_MODE_READ_KEY   = "user_mode_read";
    static constexpr std::string_view USER_MODE_WRITE_KEY  = "user_mode_write";
    static constexpr std::string_view USER_MODE_EXEC_KEY   = "user_mode_exec";
    static constexpr std::string_view GROUP_MODE_READ_KEY  = "group_mode_read";
    static constexpr std::string_view GROUP_MODE_WRITE_KEY = "group_mode_write";
    static constexpr std::string_view GROUP_MODE_EXEC_KEY  = "group_mode_exec";
    static constexpr std::string_view UNIV_MODE_READ_KEY   = "univ_mode_read";
    static constexpr std::string_view UNIV_MODE_WRITE_KEY  = "univ_mode_write";
    static constexpr std::string_view UNIV_MODE_EXEC_KEY   = "univ_mode_exec";
    static constexpr std::string_view LAST_ACCESS_KEY      = "last_access";
    static constexpr std::string_view LAST_MODIFY_KEY      = "last_modify";
    static constexpr std::string_view LAST_CHANGE_KEY      = "last_change";
    static constexpr std::string_view UID_KEY              = "uid";
    static constexpr std::string_view GID_KEY              = "gid";
    static constexpr std::string_view FILE_SIZE_KEY        = "file_size";

    INODE inode;
    std::string base_name;
    std::string file_type;
    fs::Mode mode;
    time_t last_access;
    time_t last_modify;
    time_t last_change;
    int uid;
    int gid;
    std::optional<int> file_size;

    private:
    INODE parent_dir_inode;
};

class FSMetadataCollection {
    public:
    static std::optional<INODE> create_entry(const char* path, FSMetadataCollectionEntry& fs_metadata_collection_entry);
    static std::optional<INODE>
    create_entry_with_dir_parent(INODE dir_parent_inode, FSMetadataCollectionEntry& fs_metadata_collection_entry);
    static std::optional<FSMetadataCollectionEntry> search_by_inode(INODE inode);
    static std::optional<FSMetadataCollectionEntry> search_by_base_name(std::string& base_name);
    static std::optional<INODE> reach_parent_inode(const char* path);
    static std::optional<FSMetadataCollectionEntry> get_entry_from_path(const char* path);
    static FSMetadataCollectionEntry get_md_entry_of_fs_root_dir();
    static std::vector<FSMetadataCollectionEntry> get_child_md_entries_of_parent_dir(INODE parent_dir_inode);
    static std::optional<FSMetadataCollectionEntry>
    get_child_entry_from_parent_inode(INODE parent_dir_inode, std::string& child_base_name);
    static void update_md_entry_size(INODE inode, int size);
    static bool inode_is_parent(INODE parent_dir_inode, INODE child_inode);
    static constexpr std::string_view NAME = "fs_metadata";
    static void remove_mtn_dir();
};

} // namespace mongo

#endif
