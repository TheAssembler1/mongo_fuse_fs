// NOTE: needed to establish fuse version early
#define FUSE_USE_VERSION 35
#include <fuse.h>

#include "fs/handler.h"

int main(int argc, char *argv[]) {
  fuse_config config;
  config.nullpath_ok = true;
  return fuse_main(argc, argv, &fs::Operations::mongo_fuse_fs_callbacks, NULL);
}
