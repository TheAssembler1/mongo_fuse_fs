#pragma once

// NOTE: needed to establish fuse version early
#define FUSE_USE_VERSION 35
#include <fuse.h>

namespace fs {
  
  class Operations {
    static int getattr(const char* path, struct stat* stat, fuse_file_info* fuse_file_info);
    static int readlink(const char*, char*, size_t);
    static int mknod(const char*, mode_t, dev_t); 
    static int mkdir(const char*, mode_t);
    static int unlink(const char*);
    static int rmdir(const char*);
    static int symlink(const char*, const char*);
    static int rename(const char*, const char*, unsigned int);
    static int link(const char*, const char*);
    static int chmod(const char*, mode_t, fuse_file_info*);
    static int chown(const char*, uid_t, gid_t, fuse_file_info*);
    static int truncate(const char*, off_t, fuse_file_info*);
    static int open(const char*, fuse_file_info*);
    static int read(const char*, char*, size_t, off_t, fuse_file_info*);
    static int write(const char*, const char*, size_t, off_t, fuse_file_info*);
    static int statfs(const char*, struct statvfs*);
    static int flush(const char*, fuse_file_info*);
    static int release(const char*, fuse_file_info*);
    static int fsync(const char*, int, fuse_file_info*);
    static int setxattr(const char*, const char*, const char*, size_t, int);
    static int getxattr(const char*, const char*, char*, size_t);
    static int listxattr(const char*, char*, size_t);
    static int removexattr(const char*, const char*);
    static int opendir(const char* path, fuse_file_info*);
    static int readdir(const char* path, void* data, fuse_fill_dir_t filler, off_t offset, fuse_file_info* ffi, fuse_readdir_flags fdf);
    static int releasedir(const char*, fuse_file_info*);
    static int fsyncdir(const char*, int, fuse_file_info*);
    static void* init(fuse_conn_info*, fuse_config*);
    static void destroy(void*);
    static int access(const char*, int);
    static int create(const char* path, mode_t mode, fuse_file_info* ffi);
    static int lock(const char*, fuse_file_info*, int, flock*);
    static int utimens(const char*, const timespec*, fuse_file_info*);
    static int bmap(const char*, size_t, uint64_t*);
    static int ioctl(const char*, unsigned int, void*, fuse_file_info*, unsigned int, void*);
    static int poll(const char*, fuse_file_info*, fuse_pollhandle*, unsigned*);
    static int write_buf(const char*, fuse_bufvec*, off_t, fuse_file_info*);
    static int read_buf(const char*, fuse_bufvec**, size_t, off_t, fuse_file_info*);
    static int flock(const char*, fuse_file_info*, int);
    static int fallocate(const char*, int, off_t, off_t, fuse_file_info*);
    static ssize_t copy_file_range(const char*, fuse_file_info*, off_t, off_t, const char*, fuse_file_info*, off_t, size_t, int);
    static off_t lseek(const char*, off_t, int, fuse_file_info*);
    
    public:
      static constexpr struct fuse_operations mongo_fuse_fs_callbacks = {
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
        .fallocate = fallocate,
      };


      static constexpr struct fuse_config mongo_fuse_fs_config  {
        .nullpath_ok = true,
      };
  };

};


