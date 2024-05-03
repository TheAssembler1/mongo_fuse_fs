// NOTE: needed to establish fuse version early
#define FUSE_USE_VERSION 35
#include <fuse.h>

#include "fs_handler/fs_handler.h"

int main(int argc, char *argv[]) {
  return fuse_main(argc, argv, &mongo_fuse_fs::operations::mongo_fuse_fs_callbacks, NULL);
}

