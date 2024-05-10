#include "collection_helper.h"

using namespace mongo;

void CollectionHelper::write_fuse_bufvec_to_mongo(const char* path, fuse_bufvec& f_bvec, fuse_file_info& ffi) {
  std::cout << "write blocks to mongo with fuse buffer vector fd: " << ffi.fh << std::endl;
  std::cout << "write blocks to mongo with fuse buffer vector with number of buffers: " << f_bvec.count << std::endl;
  std::cout << "write blocks to mongo with fuse buffer vector with offset: " << f_bvec.off << std::endl;

  assert(f_bvec.off == 0);

  if(FUSE_BUF_FD_SEEK) {
    std::cout << "seek pointer set" << std::endl;
  } else {
    std::cout << "seek pointer not set" << std::endl;
  }

  for(; f_bvec.idx < f_bvec.count; f_bvec.idx++) {
    std::cout << "writing buffer with idx: " << f_bvec.idx << std::endl;

    fuse_buf f_buf = f_bvec.buf[f_bvec.idx];
    assert(f_buf.fd == 0);

    int buf_size = f_buf.size;
    int starting_file_pos = f_buf.pos;

    std::cout << "writing buffer with size: " << buf_size << std::endl;
    std::cout << "starting file position offset: " << starting_file_pos << std::endl;

    // NOTE: writing all full blocks
    char* test_buffer = (char*)calloc(10000, sizeof(char));

    std::cout << "buffer size: " << buf_size << std::endl;
    std::cout << "current position: " << f_buf.pos << std::endl;
    std::cout << "starting position: " << starting_file_pos << std::endl;
    std::cout << "=======================================================" << std::endl;

    while(f_buf.pos - starting_file_pos < buf_size) {
      int block_offset = f_buf.pos % BLOCK_SIZE;
      int bytes_to_write = 0;
      int remaining_bytes = buf_size - f_buf.pos - starting_file_pos;

      if(remaining_bytes > BLOCK_SIZE) {
        std::cout << "more block to write after current block" << std::endl;
        bytes_to_write = BLOCK_SIZE - block_offset;
      } else {
        std::cout << "writing remaining bytes" << std::endl;
        bytes_to_write = remaining_bytes;
      }

      std::cout << "writing to block offset: " << block_offset << std::endl;
      std::cout << "writing size in bytes: " << bytes_to_write << std::endl;
      std::cout << "current file position: " << f_buf.pos << std::endl;
      std::cout << "remaining bytes: " << remaining_bytes << std::endl;

      //auto lookup_entry = FSLookupCollection::read_entry_with_inode_order(ffi.fh, block_offset).value();
      //sauto data_entry = FSDataCollection::read_entry(lookup_entry.fs_data_id).value();

      for(int i = 0; i < bytes_to_write; i++) {
        std::cout << "HERE" << std::endl;
        std::cout << ((char*)f_buf.mem)[i] << std::endl;
        test_buffer[i + block_offset] = ((char*)f_buf.mem)[f_buf.pos];
      }

      f_buf.pos += bytes_to_write;
    }

    for(int i = 0; i < 1000; i++) {
      if(!test_buffer[i]) {
        break;
      }
      std::cout << test_buffer[i] << std::endl;
    }

    free(test_buffer);
  }
}
