#include <iostream>

#include "fs_handler.h"

#define U {/*throw std::runtime_error("function unimplemented");*/std::cerr << "HERE" << std::endl; return 0;}

namespace operations = mongo_fuse_fs::operations;

int operations::getattr(const char* path, struct stat* stat, fuse_file_info* fuse_file_info) {
  std::cout << "getattr" << std::endl;


  stat->st_dev = 0;
  stat->st_ino = 0;
  stat->st_mode = 0;
  stat->st_nlink = 0;
  stat->st_uid = 0;
  stat->st_gid = 0;
  stat->st_rdev = 0;
  stat->st_size = 0;
  stat->st_blksize = 0;
  stat->st_blocks = 0;
std::cout << "creating mongo db instance" << std::endl;

  printf("getattr path=%s\n", path);
  stat->st_dev = 2049;
  stat->st_ino = 14450705;
  stat->st_mode = 040775;
  stat->st_nlink = 2;
  stat->st_uid = 1000;
  stat->st_gid = 1000;
  stat->st_rdev = 0;
  stat->st_size = 4096;
  stat->st_blksize = 4096;
  stat->st_blocks = 8;
  time(&(stat->st_atime));
  time(&(stat->st_mtime));
  time(&(stat->st_ctime));

  return 0;
};

int operations::readlink(const char*, char*, size_t) U;
int operations::mknod(const char*, mode_t, dev_t) U; 
int operations::mkdir(const char*, mode_t) U;
int operations::unlink(const char*) U;
int operations::rmdir(const char*) U;
int operations::symlink(const char*, const char*) U;
int operations::rename(const char*, const char*, unsigned int) U;
int operations::link(const char*, const char*) U;
int operations::chmod(const char*, mode_t, fuse_file_info*) U;
int operations::chown(const char*, uid_t, gid_t, fuse_file_info*) U;
int operations::truncate(const char*, off_t, fuse_file_info*) U;
int operations::open(const char*, fuse_file_info*) U;
int operations::read(const char*, char*, size_t, off_t, fuse_file_info*) U;
int operations::write(const char*, const char*, size_t, off_t, fuse_file_info*) U;
int operations::statfs(const char*, struct statvfs*) U;
int operations::flush(const char*, fuse_file_info*) U;
int operations::release(const char*, fuse_file_info*) U;
int operations::fsync(const char*, int, fuse_file_info*) U;
int operations::setxattr(const char*, const char*, const char*, size_t, int) U;
int operations::getxattr(const char*, const char*, char*, size_t) U;
int operations::listxattr(const char*, char*, size_t) U;
int operations::removexattr(const char*, const char*) U;

int operations::opendir(const char* path, fuse_file_info*) {
  std::cout << "opendir path=" << path << std::endl;

  return 0;
}

int operations::readdir(const char* path, void* data, fuse_fill_dir_t filler, off_t offset, fuse_file_info* ffi, fuse_readdir_flags fdf) {
  std::cout << "readdir path=" << path << std::endl;
  return 0;
}

int operations::releasedir(const char*, fuse_file_info*);
int operations::fsyncdir(const char*, int, fuse_file_info*) U;
void* operations::init(fuse_conn_info*, fuse_config*) U;
void operations::destroy(void*) {}; 
int operations::access(const char*, int) U;

int operations::create(const char*, mode_t, fuse_file_info*) {
  return 0;
}

int operations::lock(const char*, fuse_file_info*, int, struct flock*) U;
int operations::utimens(const char*, const timespec*, fuse_file_info*) U;
int operations::bmap(const char*, size_t, uint64_t*) U;
int operations::ioctl(const char*, unsigned int, void*, fuse_file_info*, unsigned int, void*) U;
int operations::poll(const char*, fuse_file_info*, fuse_pollhandle*, unsigned*) U;
int operations::write_buf(const char*, fuse_bufvec*, off_t, fuse_file_info*) U;
int operations::read_buf(const char*, fuse_bufvec**, size_t, off_t, fuse_file_info*) U;
int operations::flock(const char*, fuse_file_info*, int) U;
int operations::fallocate(const char*, int, off_t, off_t, fuse_file_info*) U;
ssize_t operations::copy_file_range(const char*, fuse_file_info*, off_t, off_t, const char*, fuse_file_info*, off_t, size_t, int) U;
off_t operations::lseek(const char*, off_t, int, fuse_file_info*) U;
