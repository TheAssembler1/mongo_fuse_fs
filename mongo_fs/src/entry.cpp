// NOTE: needed to establish fuse version early
#define FUSE_USE_VERSION 35
#include <fuse.h>
#include <unistd.h>

#include "fs/handler.h"
#include "mongo/mongo_manager.h"

int main(int argc, char* argv[]) {
    fuse_config config;
    config.nullpath_ok = true;

    char cwd[MAX_FS_ROOT_PATH];
    memset(cwd, 0, MAX_FS_ROOT_PATH);

    if(getcwd(cwd, MAX_FS_ROOT_PATH) != nullptr) {
        mongo::MongoManager::fs_root_path = std::string{cwd};
        std::cout << "FS root path: " << mongo::MongoManager::fs_root_path << std::endl;
    } else {
        std::cerr << "ERROR: failed to get cwd" << std::endl;
        return -1;
    }

    return fuse_main(argc, argv, &fs::Operations::mongo_fuse_fs_callbacks, NULL);
}
