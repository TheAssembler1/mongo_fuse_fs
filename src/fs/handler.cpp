#include <iostream>
#include <memory.h>

#include "handler.h"
#include "../mongo/manager.h"
#include "../mongo/connection.h"
#include "../mongo/md_entry.h"

#define FS_OPERATION_FAIL 1
#define FS_OPERATION_SUCCESS 0
#define U {std::cerr << "unimplemented method: " << __FUNCTION__ << std::endl; return FS_OPERATION_SUCCESS;}

using namespace fs;

int Operations::create(const char *path, mode_t mode, fuse_file_info *ffi) {
  std::cout << "create path " << path << std::endl;

  if(ffi) {
    std::cout << "getattr non_null ffi: " << ffi->fh << std::endl;
  } else {
    std::cout << "getattr null ffi" << std::endl;
  }

  mongo::MDEntry md_entry(path, mode, mongo::MDFileType::FILE);
  md_entry.create_entry();
  
  return FS_OPERATION_SUCCESS;
}

int Operations::getattr(const char* path, struct stat* stat, fuse_file_info* ffi) {
  memset(stat, 0, sizeof(struct stat));
  std::cout << "getattr path " << path << std::endl;

  if(ffi) {
    std::cout << "getattr non_null ffi: " << ffi->fh << std::endl;
  } else {
    std::cout << "getattr null ffi" << std::endl;
  }

  if(strcmp(path, "/") == 0) {
    stat->st_mode = S_IFDIR | 0755;
    stat->st_nlink = 0;
    return -ENOENT;
  }
  
  std::optional<mongo::MDEntry> md_entry = std::nullopt;
  if (!ffi) {
    std::cout << "ffi null searching by path" << std::endl;
    md_entry = mongo::MDEntry::search_by_path(path);
  } else {
    std::cout << "ffi null searching by path" << std::endl;
    md_entry = mongo::MDEntry::search_by_fd(ffi->fh);
  }

  if(md_entry.has_value()) {

    stat->st_mode = S_IFREG | 0644;
    stat->st_size = 0;

    return FS_OPERATION_SUCCESS;
  }

  return -ENOENT;
};

int Operations::readlink(const char*, char*, size_t) U;
int Operations::mknod(const char*, mode_t, dev_t) U; 
int Operations::mkdir(const char*, mode_t) U;
int Operations::unlink(const char*) U;
int Operations::rmdir(const char*) U;
int Operations::symlink(const char*, const char*) U;
int Operations::rename(const char*, const char*, unsigned int) U;
int Operations::link(const char*, const char*) U;
int Operations::chmod(const char*, mode_t, fuse_file_info*) U;
int Operations::chown(const char*, uid_t, gid_t, fuse_file_info*) U;
int Operations::truncate(const char*, off_t, fuse_file_info*) U;
int Operations::open(const char*, fuse_file_info*) U;
int Operations::read(const char*, char*, size_t, off_t, fuse_file_info*) U;
int Operations::write(const char*, const char*, size_t, off_t, fuse_file_info*) U;
int Operations::statfs(const char*, struct statvfs*) U;
int Operations::flush(const char*, fuse_file_info*) U;
int Operations::release(const char*, fuse_file_info*) U;
int Operations::fsync(const char*, int, fuse_file_info*) U;
int Operations::setxattr(const char*, const char*, const char*, size_t, int) U;
int Operations::getxattr(const char*, const char*, char*, size_t) U;
int Operations::listxattr(const char*, char*, size_t) U;
int Operations::removexattr(const char*, const char*) U;

int Operations::opendir(const char* path, fuse_file_info*) {
  std::cout << "opendir path=" << path << std::endl;

  return FS_OPERATION_SUCCESS;
}

int Operations::readdir(const char* path, void* data, fuse_fill_dir_t filler, off_t offset, fuse_file_info* ffi, fuse_readdir_flags fdf) { std::cout << "readdir path=" << path << std::endl;
  return FS_OPERATION_SUCCESS;
}

int Operations::releasedir(const char*, fuse_file_info*) U;
int Operations::fsyncdir(const char*, int, fuse_file_info*) U;

void* Operations::init(fuse_conn_info*, fuse_config* fg) {
  std::cout << "fs init call" << std::endl;

  std::cout << "nullpath_ok set to true" << std::endl;
  fg->nullpath_ok = true;

  mongo::Manager::init_db();


  return nullptr;
}

void Operations::destroy(void*) {}; 
int Operations::access(const char*, int) U;

int Operations::lock(const char*, fuse_file_info*, int, struct flock*) U;

int Operations::utimens(const char* path, const timespec*, fuse_file_info*) {
  std::cout << "utimens call" << path << std::endl;
  errno = ENOENT;
  return -1;
}

int Operations::bmap(const char*, size_t, uint64_t*) U;
int Operations::ioctl(const char*, unsigned int, void*, fuse_file_info*, unsigned int, void*) U;
int Operations::poll(const char*, fuse_file_info*, fuse_pollhandle*, unsigned*) U;
int Operations::write_buf(const char*, fuse_bufvec*, off_t, fuse_file_info*) U;
int Operations::read_buf(const char*, fuse_bufvec**, size_t, off_t, fuse_file_info*) U;
int Operations::flock(const char*, fuse_file_info*, int) U;
int Operations::fallocate(const char*, int, off_t, off_t, fuse_file_info*) U;
ssize_t Operations::copy_file_range(const char*, fuse_file_info*, off_t, off_t, const char*, fuse_file_info*, off_t, size_t, int) U;
off_t Operations::lseek(const char*, off_t, int, fuse_file_info*) U;
