#ifndef __FS_HELPER_H__
#define __FS_HELPER_H__

#include "../mongo/collection/types.h"
#include "../mongo/mongo_manager.h"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

namespace fs {

class FSHelper {
    public:
    static std::vector<std::string> get_path_components(const char* path);
    static std::string get_parent_dir_string(const char* c_str_path);
    static std::string get_base_name_of_path(const char* c_str_path);
    static struct stat get_stat_of_root_fs();
    static INODE get_inode_of_root_fs();
};

} // namespace fs

#endif
