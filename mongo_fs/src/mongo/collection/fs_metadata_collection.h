#pragma once

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

#include "../../fs/perm.h"
#include "../manager.h"
#include "../connection.h"
#include "fs_lookup_collection.h"
#include "fs_data_collection.h"

using bsoncxx::v_noabi::document::value;
using bsoncxx::v_noabi::document::view;
using bsoncxx::v_noabi::document::view_or_value;

namespace mongo {
    enum class MDFileType {
        FILE,
        DIR
    };

    class FSMetadataCollectionEntry {
      public:
        FSMetadataCollectionEntry(INODE inode, std::string path, std::string file_type, fs::Mode mode, time_t last_access, 
                      time_t last_modify, time_t last_change, int uid, int gid) 
                      : inode{inode}, path{path}, file_type{file_type}, mode{mode}, last_access{last_access},
                        last_modify{last_modify}, last_change{last_change}, uid{uid}, gid{gid} {}
        FSMetadataCollectionEntry(const char* _path, mode_t _mode, MDFileType file_type);

        const mode_t to_mode_t();
        const value to_document(); 

        static FSMetadataCollectionEntry bson_to_md_entry(value bson_doc);

        static constexpr std::string_view INODE_KEY = "_id";
        static constexpr std::string_view PATH_KEY = "path"; 
        static constexpr std::string_view FILE_TYPE_KEY = "file_type";
        static constexpr std::string_view FILE_TYPE_FILE_VALUE = "file";
        static constexpr std::string_view FILE_TYPE_DIR_VALUE = "dir";
        static constexpr std::string_view USER_MODE_READ_KEY = "user_mode_read";
        static constexpr std::string_view USER_MODE_WRITE_KEY = "user_mode_write";
        static constexpr std::string_view USER_MODE_EXEC_KEY = "user_mode_exec";
        static constexpr std::string_view GROUP_MODE_READ_KEY = "group_mode_read";
        static constexpr std::string_view GROUP_MODE_WRITE_KEY = "group_mode_write";
        static constexpr std::string_view GROUP_MODE_EXEC_KEY = "group_mode_exec";
        static constexpr std::string_view UNIV_MODE_READ_KEY = "univ_mode_read";
        static constexpr std::string_view UNIV_MODE_WRITE_KEY = "univ_mode_write";
        static constexpr std::string_view UNIV_MODE_EXEC_KEY = "univ_mode_exec";
        static constexpr std::string_view LAST_ACCESS_KEY = "last_access";
        static constexpr std::string_view LAST_MODIFY_KEY = "last_modify";
        static constexpr std::string_view LAST_CHANGE_KEY = "last_change";
        static constexpr std::string_view UID_KEY = "uid";
        static constexpr std::string_view GID_KEY = "gid";

        INODE inode;
        std::string path;
        std::string file_type;
        fs::Mode mode;
        time_t last_access;
        time_t last_modify;
        time_t last_change;
        int uid;
        int gid;
    };

    class FSMetadataCollection {
        public:
            static INODE create_entry(FSMetadataCollectionEntry fs_metadata_collection_entry);
            static std::optional<FSMetadataCollectionEntry> search_by_inode(INODE inode);
            static std::optional<FSMetadataCollectionEntry> search_by_path(std::string path);
            static constexpr std::string_view NAME = "fs_metadata";
    };

}
