#include "handler.h"

#define FS_OPERATION_FAIL 1
#define FS_OPERATION_SUCCESS 0

#define PREHANDLER_PRINT std::cout << "===================================" << std::endl << "executing function: " << __FUNCTION__ << " handler" << std::endl
#define POSTHANDLER_PRINT std::cout << std::endl << "done executing function: " << __FUNCTION__ << " handler" << std::endl << "===================================" << std::endl
#define U {PREHANDLER_PRINT; POSTHANDLER_PRINT; return FS_OPERATION_SUCCESS;}

using namespace fs;

int Operations::create(const char *path, mode_t mode, fuse_file_info *ffi) {
  PREHANDLER_PRINT;

  assert(path != NULL);

  if(ffi) {
    std::cout << "getattr non_null ffi: " << ffi->fh << std::endl;
  } else {
    std::cout << "getattr null ffi" << std::endl;
  }
  
  mongo::FSMetadataCollectionEntry md_entry(path, mode, mongo::MDFileType::FILE);
  mongo::FSMetadataCollection::create_entry(md_entry);
  
  POSTHANDLER_PRINT;

  return FS_OPERATION_SUCCESS;
}

int Operations::getattr(const char* path, struct stat* stat, fuse_file_info* ffi) {
  PREHANDLER_PRINT;

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
  
  std::optional<mongo::FSMetadataCollectionEntry> md_entry_opt = std::nullopt;
  if (!ffi) {
    std::cout << "ffi null searching by path: " << path << std::endl;
    md_entry_opt = mongo::FSMetadataCollection::search_by_path(path);
  } else {
    std::cout << "ffi not null searching by fd: " << ffi->fh << std::endl;
    md_entry_opt = mongo::FSMetadataCollection::search_by_inode(ffi->fh);
  }

  if(md_entry_opt.has_value()) {
    mongo::FSMetadataCollectionEntry md_entry = md_entry_opt.value();
    stat->st_ino = md_entry.inode;
    stat->st_mode = md_entry.to_mode_t();
    stat->st_size = 0;
    stat->st_gid = md_entry.gid;
    stat->st_uid = md_entry.uid;
    stat->st_atim = std::timespec{md_entry.last_access, 0};
    stat->st_ctim = std::timespec{md_entry.last_change, 0};
    stat->st_mtim = std::timespec{md_entry.last_modify, 0};
    stat->st_blksize = BLOCK_SIZE;

    return FS_OPERATION_SUCCESS;
  }
  POSTHANDLER_PRINT;

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

int Operations::open(const char* path, fuse_file_info* ffi) {
  PREHANDLER_PRINT;

  std::optional<mongo::FSMetadataCollectionEntry> md_entry_opt = std::nullopt;

  std::cout << "open called with path: " << path << std::endl;
  md_entry_opt = mongo::FSMetadataCollection::search_by_path(path);

  if(!md_entry_opt.has_value()) {
    return -ENOENT;
  }

  mongo::FSMetadataCollectionEntry md_entry = md_entry_opt.value();

  std::cout << "found doc with fd: " << md_entry.inode << std::endl;
  ffi->fh = md_entry.inode;

  // FIXME: bad string output. maybe build up string?
  std::cout << "file opened with the following flags: ";

  // exclusive file flags
  if(ffi->flags & O_RDONLY) {
    std::cout << std::endl;
    return FS_OPERATION_SUCCESS;
  }
  if(ffi->flags & O_WRONLY) {
    std::cout << "O_WRONLY ";
    return FS_OPERATION_SUCCESS;
    std::cout << std::endl;
  }

  if(ffi->flags & O_RDWR) {
    std::cout << "O_RDWR ";
  }
  if(ffi->flags & O_APPEND) {
    std::cout << "O_APPEND ";
  }
  std::cout << std::endl;
  POSTHANDLER_PRINT;

  return FS_OPERATION_SUCCESS;
};

int Operations::read(const char* path, char* ret_buf, size_t size, off_t offset, fuse_file_info* ffi) {
  PREHANDLER_PRINT;

  std::cout << "read called with fd: " << ffi->fh << std::endl;

  std::optional<mongo::FSMetadataCollectionEntry> md_entry_opt =  mongo::FSMetadataCollection::search_by_inode(ffi->fh);

  if(!md_entry_opt.has_value()) {
    return -EBADF;
  }

  mongo::FSMetadataCollectionEntry md_entry = md_entry_opt.value();

  if(md_entry.file_type == mongo::FSMetadataCollectionEntry::FILE_TYPE_DIR_VALUE) {
   return -EISDIR;  
  }

  std::vector<mongo::FSDataCollectionEntry> fs_data_collection_entires = mongo::FSDataCollection::read_all_fs_data_blocks(ffi->fh);
  POSTHANDLER_PRINT;

  return size;
}

int Operations::write(const char* path, const char* buf, size_t buf_size, off_t buf_offset, fuse_file_info* ffi) U;

int Operations::statfs(const char*, struct statvfs*) U;
int Operations::flush(const char*, fuse_file_info*) U;
int Operations::release(const char*, fuse_file_info*) U;
int Operations::fsync(const char*, int, fuse_file_info*) U;

#ifdef HAVE_SETXATTR
int Operations::setxattr(const char*, const char*, const char*, size_t, int) U;
int Operations::getxattr(const char* path, const char* name, char* value, size_t size) U;
int Operations::listxattr(const char*, char*, size_t) U;
int Operations::removexattr(const char*, const char*) U;
#endif

int Operations::opendir(const char* path, fuse_file_info*) {
  PREHANDLER_PRINT;
  std::cout << "opendir path=" << path << std::endl;
  POSTHANDLER_PRINT;
  return FS_OPERATION_SUCCESS;
}

int Operations::readdir(const char* path, void* data, fuse_fill_dir_t filler, off_t offset, fuse_file_info* ffi, fuse_readdir_flags fdf) { std::cout << "readdir path=" << path << std::endl;
  PREHANDLER_PRINT;
  POSTHANDLER_PRINT;
  return FS_OPERATION_SUCCESS;
}

int Operations::releasedir(const char*, fuse_file_info*) U;
int Operations::fsyncdir(const char*, int, fuse_file_info*) U;

void* Operations::init(fuse_conn_info*, fuse_config* fg) {
  PREHANDLER_PRINT;

  std::cout << "fs init call" << std::endl;

  std::cout << "nullpath_ok set to true" << std::endl;
  fg->nullpath_ok = mongo_fuse_fs_config.nullpath_ok;
  fg->use_ino = mongo_fuse_fs_config.use_ino;

  mongo::Manager::init_db();

  POSTHANDLER_PRINT;
  return nullptr;
}

void Operations::destroy(void*) {}; 
int Operations::access(const char*, int) U;

int Operations::lock(const char*, fuse_file_info*, int, struct flock*) U;

int Operations::utimens(const char* path, const timespec*, fuse_file_info*) {
  PREHANDLER_PRINT;
  POSTHANDLER_PRINT;
  return FS_OPERATION_SUCCESS;
}

int Operations::bmap(const char*, size_t, uint64_t*) U;
int Operations::ioctl(const char*, unsigned int, void*, fuse_file_info*, unsigned int, void*) U;
int Operations::poll(const char*, fuse_file_info*, fuse_pollhandle*, unsigned*) U;

int Operations::write_buf(const char* path, fuse_bufvec* f_bvec, off_t buf_offset, fuse_file_info* ffi)  {
  PREHANDLER_PRINT;

  std::cout << "write_buf called with fd: " << ffi->fh << std::endl;
  std::cout << "number of buffers: " << f_bvec->count << std::endl;
  std::cout << "write_buf offset: " << f_bvec->off << std::endl;

  for(int i = 0; i < f_bvec->count; i++) {
    auto f_b = f_bvec->buf[i];

    assert(ffi->fh != 0);

    std::cout << "write_buf called with fd: " << (int)ffi->fh << std::endl;
    auto md_entry_opt = mongo::FSMetadataCollection::search_by_inode(ffi->fh);

    if(!md_entry_opt.has_value()) {
      std::cerr << "md entry has no value in write_buf" << std::endl;
    }

    auto md_entry = md_entry_opt.value();
    mongo::FSDataCollectionEntry fs_data_entry = mongo::FSDataCollectionEntry{(long int)f_b.size, (char*)f_b.mem};
    std::optional<int> bytes_written_opt = mongo::FSDataCollection::create_entry(ffi->fh, fs_data_entry);

    if(!bytes_written_opt.has_value() != f_b.size) {
      std::cerr << "bytes written != to buffer size" << std::endl;
      std::cerr << "bytes written: " << bytes_written_opt.value() << std::endl;
    }
  }

  POSTHANDLER_PRINT;
  return FS_OPERATION_SUCCESS;
};

int Operations::read_buf(const char*, fuse_bufvec**, size_t, off_t, fuse_file_info*) U;
int Operations::flock(const char*, fuse_file_info*, int) U;
int Operations::fallocate(const char*, int, off_t, off_t, fuse_file_info*) U;
ssize_t Operations::copy_file_range(const char*, fuse_file_info*, off_t, off_t, const char*, fuse_file_info*, off_t, size_t, int) U;
off_t Operations::lseek(const char*, off_t, int, fuse_file_info*) U;
