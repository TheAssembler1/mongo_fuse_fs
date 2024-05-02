#include <exception>
#include <stdexcept>
#include <iostream>

#define FUSE_USE_VERSION 31
#include <fuse.h>

#define U {throw std::runtime_error("function unimplemented");}

namespace MongoFuseFS {

  int getattr(const char*, struct stat*) U;
  int readlink(const char*, char*, size_t) U;
  int mknod(const char*, mode_t, dev_t) U; 
  int mkdir(const char*, mode_t) U;
  int unlink(const char*) U;
  int rmdir(const char*) U;
  int symlink(const char*, const char*) U;
  int rename(const char*, const char*) U;
  int link(const char*, const char*) U;
  int chmod(const char*, mode_t) U;
  int chown(const char*, uid_t, gid_t) U;
  int truncate(const char*, off_t) U;
  int open(const char*, struct fuse_file_info*) U;
  int read(const char*, char*, size_t, off_t, struct fuse_file_info*) U;
  int write(const char*, const char*, size_t, off_t, struct fuse_file_info*) U;
  int statfs(const char*, struct statvfs*) U;
  int flush(const char*, struct fuse_file_info*) U;
  int release(const char*, struct fuse_file_info*) U;
  int fsync(const char*, int, struct fuse_file_info*) U;
  int setxattr(const char*, const char*, const char*, size_t, int) U;
  int getxattr(const char*, const char*, char*, size_t) U;
  int listxattr(const char*, char*, size_t) U;
  int removexattr(const char*, const char*) U;
  int opendir(const char*, struct fuse_file_info*) U;
  int readdir(const char*, void*, fuse_fill_dir_t, off_t, struct fuse_file_info*) U;
  int releasedir(const char*, struct fuse_file_info*);
  int fsyncdir(const char*, int, struct fuse_file_info*) U;
  void* init(struct fuse_conn_info*) U;
  void destroy(void*) U; 
  int access(const char*, int) U;
  int create(const char*, mode_t, struct fuse_file_info*) U;
  int lock(const char*, struct fuse_file_info*, int, struct flock*) U;
  int utimens(const char*, const struct timespec*) U;
  int bmap(const char*, size_t, uint64_t*) U;
  int ioctl(const char*, int, void*, struct fuse_file_info*, unsigned int, void*) U;
  int poll(const char*, struct fuse_file_info*, struct fuse_pollhandle*, unsigned*) U;
  int write_buf(const char*, struct fuse_bufvec*, off_t, struct fuse_file_info*) U;
  int read_buf(const char*, struct fuse_bufvec**, size_t, off_t, struct fuse_file_info*) U;
  int flock(const char*, struct fuse_file_info*, int) U;
  int fallocate(const char*, int, off_t, off_t, struct fuse_file_info*) U;
  ssize_t copy_file_range(const char*, struct fuse_file_info*, off_t, off_t, const char*, struct fuse_file_info*, off_t, size_t, int) U;
  off_t lseek(const char*, off_t, int, struct fuse_file_info*) U;

};


static struct fuse_operations myfs_ops = {
  .getattr = MongoFuseFS::getattr,
  .readlink = MongoFuseFS::readlink,
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
  .removexattr = MongoFuseFS::removexattr,
  .opendir = MongoFuseFS::opendir,
  .readdir = MongoFuseFS::readdir,
  .fsyncdir = MongoFuseFS::fsyncdir,
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
  .fallocate = MongoFuseFS::fallocate,
};

int main(int argc, char *argv[]) {
  return fuse_main(argc, argv, &myfs_ops, NULL);
}

