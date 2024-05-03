#pragma once

// NOTE: needed to establish fuse version early
#define FUSE_USE_VERSION 35
#include <fuse.h>


namespace mongo_fuse_fs {
  
  namespace operations {

    int getattr(const char* path, struct stat* stat, fuse_file_info* fuse_file_info);
    int readlink(const char*, char*, size_t);
    int mknod(const char*, mode_t, dev_t); 
    int mkdir(const char*, mode_t);
    int unlink(const char*);
    int rmdir(const char*);
    int symlink(const char*, const char*);
    int rename(const char*, const char*, unsigned int);
    int link(const char*, const char*);
    int chmod(const char*, mode_t, fuse_file_info*);
    int chown(const char*, uid_t, gid_t, fuse_file_info*);
    int truncate(const char*, off_t, fuse_file_info*);
    int open(const char*, fuse_file_info*);
    int read(const char*, char*, size_t, off_t, fuse_file_info*);
    int write(const char*, const char*, size_t, off_t, fuse_file_info*);
    int statfs(const char*, struct statvfs*);
    int flush(const char*, fuse_file_info*);
    int release(const char*, fuse_file_info*);
    int fsync(const char*, int, fuse_file_info*);
    int setxattr(const char*, const char*, const char*, size_t, int);
    int getxattr(const char*, const char*, char*, size_t);
    int listxattr(const char*, char*, size_t);
    int removexattr(const char*, const char*);
    int opendir(const char* path, fuse_file_info*);
    int readdir(const char* path, void* data, fuse_fill_dir_t filler, off_t offset, fuse_file_info* ffi, fuse_readdir_flags fdf);
    int releasedir(const char*, fuse_file_info*);
    int fsyncdir(const char*, int, fuse_file_info*);
    void* init(fuse_conn_info*, fuse_config*);
    void destroy(void*);
    int access(const char*, int);
    int create(const char*, mode_t, fuse_file_info*);
    int lock(const char*, fuse_file_info*, int, flock*);
    int utimens(const char*, const timespec*, fuse_file_info*);
    int bmap(const char*, size_t, uint64_t*);
    int ioctl(const char*, unsigned int, void*, fuse_file_info*, unsigned int, void*);
    int poll(const char*, fuse_file_info*, fuse_pollhandle*, unsigned*);
    int write_buf(const char*, fuse_bufvec*, off_t, fuse_file_info*);
    int read_buf(const char*, fuse_bufvec**, size_t, off_t, fuse_file_info*);
    int flock(const char*, fuse_file_info*, int);
    int fallocate(const char*, int, off_t, off_t, fuse_file_info*);
    ssize_t copy_file_range(const char*, fuse_file_info*, off_t, off_t, const char*, fuse_file_info*, off_t, size_t, int);
    off_t lseek(const char*, off_t, int, fuse_file_info*);

    inline struct fuse_operations mongo_fuse_fs_callbacks = {
      .getattr = getattr,
      .readlink = readlink,
      .mknod = mknod,
      .mkdir = mkdir,
      .unlink = unlink,
      .rmdir = rmdir,
      .symlink = symlink,
      .rename = rename,
      .link = link,
      .chmod = chmod,
      .chown = chown,
      .truncate = truncate,
      .open = open,
      .read = read,
      .write = write,
      .statfs = statfs,
      .flush = flush,
      .release = release,
      .fsync = fsync,
      .setxattr = setxattr,
      .getxattr = getxattr,
      .listxattr = listxattr,
      .removexattr = removexattr,
      .opendir = opendir,
      .readdir = readdir,
      .fsyncdir = fsyncdir,
      .init = init,
      .destroy = destroy,
      .access = access,
      .create = create,
      .lock = lock,
      .utimens = utimens,
      .bmap = bmap,
      .ioctl = ioctl,
      .poll = poll,
      .write_buf = write_buf,
      .read_buf = read_buf,
      .flock = flock,
      .fallocate = fallocate
    };

    inline struct fuse_config mongo_fuse_fs_config  {
      .nullpath_ok = true,
    };
  }

};


