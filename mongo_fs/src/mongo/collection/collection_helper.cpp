#include "collection_helper.h"

using namespace mongo;

void CollectionHelper::write_blocks_to_mongo(const char* path, fuse_bufvec& f_bvec, fuse_file_info& ffi) {
  std::cout << "write blocks to mongo with fuse buffer vector fd: " << ffi.fh << std::endl;
  std::cout << "write blocks to mongo with fuse buffer vector with number of buffers: " << f_bvec.count << std::endl;
  std::cout << "write blocks to mongo with fuse buffer vector with offset: " << f_bvec.off << std::endl;

  if(FUSE_BUF_FD_SEEK) {
    std::cout << "seek pointer set" << std::endl;
  }

  for(; f_bvec.idx < f_bvec.count; f_bvec.idx++) {
    std::cout << "writing buffer with idx: " << f_bvec.idx << std::endl;
  }
}
