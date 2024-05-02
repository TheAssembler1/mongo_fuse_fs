#include <exception>
#include <stdexcept>
#include <iostream>
#include <cstdio>
#include <memory.h>
#define FUSE_USE_VERSION 35
#include <fuse.h>
#include <sys/stat.h>

#define U {/*throw std::runtime_error("function unimplemented");*/std::cerr << "HERE" << std::endl; return 0;}

namespace MongoFuseFS {

  int getattr(const char* path, struct stat* stat, fuse_file_info* fuse_file_info) {
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

    printf("getattr path=%s\n", path);
    memset(stat, 0, sizeof(stat));
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
  int readlink(const char*, char*, size_t) U;
  int mknod(const char*, mode_t, dev_t) U; 
  int mkdir(const char*, mode_t) U;
  int unlink(const char*) U;
  int rmdir(const char*) U;
  int symlink(const char*, const char*) U;
  int rename(const char*, const char*, unsigned int) U;
  int link(const char*, const char*) U;
  int chmod(const char*, mode_t, fuse_file_info*) U;
  int chown(const char*, uid_t, gid_t, fuse_file_info*) U;
  int truncate(const char*, off_t, fuse_file_info*) U;
  int open(const char*, fuse_file_info*) U;
  int read(const char*, char*, size_t, off_t, fuse_file_info*) U;
  int write(const char*, const char*, size_t, off_t, fuse_file_info*) U;
  int statfs(const char*, struct statvfs*) U;
  int flush(const char*, fuse_file_info*) U;
  int release(const char*, fuse_file_info*) U;
  int fsync(const char*, int, fuse_file_info*) U;
  int setxattr(const char*, const char*, const char*, size_t, int) U;
  int getxattr(const char*, const char*, char*, size_t) U;
  int listxattr(const char*, char*, size_t) U;
  int removexattr(const char*, const char*) U;
  int opendir(const char* path, fuse_file_info*) {
    std::cout << "opendir path=" << path << std::endl;

    return 0;
  }
  int readdir(const char* path, void*, fuse_fill_dir_t, off_t, fuse_file_info*, fuse_readdir_flags) {
    std::cout << "readdir path=" << path << std::endl;
    return 0;
  }
  int releasedir(const char*, fuse_file_info*);
  int fsyncdir(const char*, int, fuse_file_info*) U;
  void* init(fuse_conn_info*, fuse_config*) U;
  void destroy(void*) {}; 
  int access(const char*, int) U;
  int create(const char*, mode_t, fuse_file_info*) U;
  int lock(const char*, fuse_file_info*, int, flock*) U;
  int utimens(const char*, const timespec*, fuse_file_info*) U;
  int bmap(const char*, size_t, uint64_t*) U;
  int ioctl(const char*, unsigned int, void*, fuse_file_info*, unsigned int, void*) U;
  int poll(const char*, fuse_file_info*, fuse_pollhandle*, unsigned*) U;
  int write_buf(const char*, fuse_bufvec*, off_t, fuse_file_info*) U;
  int read_buf(const char*, fuse_bufvec**, size_t, off_t, fuse_file_info*) U;
  int flock(const char*, fuse_file_info*, int) U;
  int fallocate(const char*, int, off_t, off_t, fuse_file_info*) U;
  ssize_t copy_file_range(const char*, fuse_file_info*, off_t, off_t, const char*, fuse_file_info*, off_t, size_t, int) U;
  off_t lseek(const char*, off_t, int, fuse_file_info*) U;

};


static struct fuse_operations myfs_ops = {
  .getattr = MongoFuseFS::getattr,
  /*.readlink = MongoFuseFS::readlink,
  .mknod = MongoFuseFS::mknod,
  .mkdir = MongoFuseFS::mkdir,
  .unlink = MongoFuseFS::unlink,
  .rmdir = MongoFuseFS::rmdir,
  .symlink = MongoFuseFS::symlink,
  .rename = MongoFuseFS::rename,
  .link = MongoFuseFS::link,
  .chmod = MongoFuseFS::chmod,
  .chown = MongoFuseFS::chown,
  .truncate = MongoFuseFS::truncate,
  .open = MongoFuseFS::open,
  .read = MongoFuseFS::read,
  .write = MongoFuseFS::write,
  .statfs = MongoFuseFS::statfs,
  .flush = MongoFuseFS::flush,
  .release = MongoFuseFS::release,
  .fsync = MongoFuseFS::fsync,
  .setxattr = MongoFuseFS::setxattr,
  .getxattr = MongoFuseFS::getxattr,
  .listxattr = MongoFuseFS::listxattr,
  .removexattr = MongoFuseFS::removexattr,*/
  .opendir = MongoFuseFS::opendir,
  .readdir = MongoFuseFS::readdir,
  /*.fsyncdir = MongoFuseFS::fsyncdir,
  .init = MongoFuseFS::init,
  .destroy = MongoFuseFS::destroy,
  .access = MongoFuseFS::access,
  .create = MongoFuseFS::create,
  .lock = MongoFuseFS::lock,
  .utimens = MongoFuseFS::utimens,
  .bmap = MongoFuseFS::bmap,
  .ioctl = MongoFuseFS::ioctl,
  .poll = MongoFuseFS::poll,
  .write_buf = MongoFuseFS::write_buf,
  .read_buf = MongoFuseFS::read_buf,
  .flock = MongoFuseFS::flock,
  .fallocate = MongoFuseFS::fallocate,*/
};

int main(int argc, char *argv[]) {
  //freopen("fuse_stdout.log", "w", stdout);
  //freopen("fuse_stderr.log", "w", stderr);
  
  std::cout << "mongo fuse filesystem mounted" << std::endl;

  return fuse_main(argc, argv, &myfs_ops, NULL);
}

