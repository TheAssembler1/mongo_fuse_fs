#pragma once

#include "../fs/perm.h"

#include <iostream>

// NOTE: for metadata entries =================================
// holds entries to metadata 
#define MD_COLLECTION_NAME "fs_metadata"
// the id of the root directory document in the metadata collection
#define MD_DOC_PATH_KEY "path"
#define MD_ROOT_DIR_DOC_PATH_VALUE "/"
// specifies whether metadata entries is a file or dir
#define MD_TYPE_KEY "type"
#define MD_FS_DIR_MD_TYPE "dir"
#define MD_FS_FILE_MD_TYPE "file"

#define LAST_ACCESS_KEY "last_access"
#define LAST_MODIFY_KEY "last_modify"
#define LAST_CHANGE_KEY "last_change"
// specifies permission bits inside metadata entries
#define USER_PERM_PREFIX_KEY "user_"
#define GROUP_PERM_PREFIX_KEY "group_"
#define UNIV_PERM_PREFIX_KEY "group_"

// user perms
#define USER_PERM_READ_KEY USER_PERM_PREFIX_KEY "read"
#define USER_PERM_WRITE_KEY USER_PERM_PREFIX_KEY "write"
#define USER_PERM_EXEC_KEY USER_PERM_PREFIX_KEY "exec"

// group perms
#define GROUP_PERM_READ_KEY GROUP_PERM_PREFIX_KEY "read"
#define GROUP_PERM_WRITE_KEY GROUP_PERM_PREFIX_KEY "write"
#define GROUP_PERM_EXEC_KEY GROUP_PERM_PREFIX_KEY "exec"

// univ perms
#define UNIV_PERM_READ_KEY UNIV_PERM_PREFIX_KEY "read"
#define UNIV_PERM_WRITE_KEY UNIV_PERM_PREFIX_KEY "write"
#define UNIV_PERM_EXEC_KEY UNIV_PERM_PREFIX_KEY "exec"

// uid, and gid keys
#define UID_KEY "uid"
#define GID_KEY "gid"

typedef uint64_t INODE;

using bsoncxx::v_noabi::document::value;

namespace mongo {
    enum class MDFileType {
        FILE,
        DIR
    };


    class MDEntry {
        public:
            MDEntry(long fd, std::string path, std::string md_type, fs::Mode mode, time_t last_access, 
                    time_t last_modify, time_t last_change, int uide, int gid) 
                    : fd{fd}, path{path}, md_type{md_type}, mode{mode}, last_access{last_access},
                      last_modify{last_modify}, last_change{last_change}, uid{uid}, gid{gid} {}
            MDEntry(const char* _path, mode_t _mode, MDFileType file_type);
            const INODE create_entry();
            const mode_t to_mode_t();

            static std::optional<MDEntry> search_by_fd(INODE fd);
            static std::optional<MDEntry> search_by_path(std::string path);
            static MDEntry bson_to_md_entry(value bson_doc);

        private:
            long fd;
            std::string path;
            std::string md_type;
            fs::Mode mode;
            time_t last_access;
            time_t last_modify;
            time_t last_change;
            int uid;
            int gid;
    };

}