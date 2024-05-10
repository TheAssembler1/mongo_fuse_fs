#include "collection_helper.h"

using namespace mongo;

void CollectionHelper::create_next_data_lookup_entries(INODE inode) {
  FSDataCollectionEntry data_entry{};
  auto data_entry_id = FSDataCollection::create_entry(data_entry).value();
  FSLookupCollectionEntry lookup_entry{inode, data_entry_id};
  FSLookupCollection::create_next_entry(lookup_entry);
}

void CollectionHelper::print_all_file(INODE inode) {
  std::cout << "read all blocks for inode: " << inode << std::endl;

  auto fs_ids = FSLookupCollection::get_fs_data_ids(inode);

  std::cout << "number of blocks found: " << fs_ids.size() << std::endl;

  std::cout << "file output: " << std::endl;

  for(auto id: fs_ids) {
    auto entry = FSDataCollection::read_entry(id).value();

    for(int i = 0; i < BLOCK_SIZE; i++) {
      std::cout << entry.buf[i];
    }
  }

  std::cout << std::endl << "end file output" << std::endl;
}

void CollectionHelper::write_fuse_bufvec_to_mongo(const char* path, fuse_bufvec& f_bvec, fuse_file_info& ffi) {
  std::cout << "write blocks to mongo with fuse buffer vector fd: " << ffi.fh << std::endl;
  std::cout << "write blocks to mongo with fuse buffer vector with number of buffers: " << f_bvec.count << std::endl;
  std::cout << "write blocks to mongo with fuse buffer vector with offset: " << f_bvec.off << std::endl;

  // NOTE: gettting metadata for file
  auto metadata_entry = FSMetadataCollection::search_by_inode(ffi.fh).value();

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

    int metadata_file_size = metadata_entry.file_size.value();

    std::cout << "buffer size: " << buf_size << std::endl;
    std::cout << "current position: " << f_buf.pos << std::endl;
    std::cout << "starting position: " << starting_file_pos << std::endl;
    std::cout << "starting file size: " << metadata_file_size << std::endl;

    int additional_bytes_needed = f_buf.pos + buf_size - metadata_file_size;
    int total_bytes_after_write = additional_bytes_needed + metadata_file_size;
    int total_blocks_needed = ((metadata_file_size + additional_bytes_needed) / BLOCK_SIZE) + 1;
    int current_blocks = (metadata_file_size / BLOCK_SIZE) + 1;
    int additional_blocks_needed = total_blocks_needed - current_blocks;

    std::cout << "current blocks: " << current_blocks << std::endl;
    std::cout << "total blocks needed: " << total_blocks_needed << std::endl;;

    // NOTE: creating lookup and data entriescreate_next_data_lookup_entries
    for(int i = 0; i < additional_blocks_needed; i++) {
      create_next_data_lookup_entries((INODE)ffi.fh);
    }

    std::cout << "-------------------------------------------------------" << std::endl;

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

      auto lookup_entry = FSLookupCollection::read_entry_with_inode_order(ffi.fh, block_offset).value();
      auto data_entry = FSDataCollection::read_entry(lookup_entry.fs_data_id).value();

      for(int i = 0; i < bytes_to_write; i++) {
        data_entry.buf[i + block_offset] = ((const char*)f_buf.mem)[f_buf.pos + i];
        FSDataCollection::update_entry(data_entry);
      }

      f_buf.pos += bytes_to_write;

      FSMetadataCollection::update_md_entry_size((INODE)ffi.fh, total_bytes_after_write);
    }
  }

  print_all_file(ffi.fh);
}
