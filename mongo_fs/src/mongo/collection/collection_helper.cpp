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

int CollectionHelper::write_fuse_bufvec_to_mongo(const char* path, fuse_bufvec& f_bvec, off_t offset, fuse_file_info& ffi) {
  assert(f_bvec.off == 0);

  // NOTE: gettting metadata for file
  auto metadata_entry = FSMetadataCollection::search_by_inode(ffi.fh).value();

  int cur_file_size = metadata_entry.file_size.value();
  int total_bytes_written = 0;

  for(; f_bvec.idx < f_bvec.count; f_bvec.idx++) {
    fuse_buf f_buf = f_bvec.buf[f_bvec.idx];
    assert(f_buf.fd == 0);

    int cur_blocks = (cur_file_size / BLOCK_SIZE) + 1;
    int starting_fp = offset;
    int cur_fp = starting_fp;
    int buf_size = f_buf.size;

    // NOTE: check if we need more blocks
    int additional_blocks_needed = 0;
    if(offset + f_buf.size > cur_file_size) {
        int blocks_needed = ((offset + f_buf.size) / BLOCK_SIZE) + 1;
        additional_blocks_needed = blocks_needed - cur_blocks;

        // NOTE: increase cur_file_size for potential next iteration
        cur_file_size += offset + f_buf.size - cur_file_size;

        std::cout << "creating " << additional_blocks_needed << " additional blocks" << std::endl;

        // NOTE: creating lookup and data entriescreate_next_data_lookup_entries
        for(int i = 0; i < additional_blocks_needed; i++) {
          create_next_data_lookup_entries((INODE)ffi.fh);
        }
    }

    while(cur_fp - starting_fp < buf_size) {
      int block_order = cur_fp / BLOCK_SIZE;

      std::cout << "writing block with order: " << block_order << std::endl;
      auto lookup_entry = FSLookupCollection::read_entry_with_inode_order(ffi.fh, block_order).value();
      std::cout << "writing block with fs data id: " << lookup_entry.fs_data_id << std::endl;
      auto data_entry = FSDataCollection::read_entry(lookup_entry.fs_data_id).value();

      // NOTE: start at current position within block based on FP
      for(int pos_in_block = cur_fp % BLOCK_SIZE; pos_in_block < BLOCK_SIZE;) {
        // NOTE: ensuring we don't overwite in cur block
        if(cur_fp - starting_fp >= buf_size) {
            break;
        }

        // NOTE: read buffer into mongon buffer
        data_entry.buf[pos_in_block] = ((const char*)f_buf.mem)[cur_fp - starting_fp];

        // NOTE: updating pos and total bytes written
        total_bytes_written++;
        cur_fp++;
        pos_in_block++;
      }

      // NOTE: update data entry with new data
      FSDataCollection::update_entry(data_entry);
    }
  }

  FSMetadataCollection::update_md_entry_size((INODE)ffi.fh, cur_file_size);
  return total_bytes_written;
}

// FIXME: currently copying bytes over could just point to bson buffers in f bvec...
void CollectionHelper::read_mongo_to_fuse_bufvec(const char* path, fuse_bufvec** _f_bvec, size_t size, off_t offset, fuse_file_info& ffi) {
    // NOTE: gettting metadata for file
    auto metadata_entry = FSMetadataCollection::search_by_inode(ffi.fh).value();

    std::cout << "num bytes requested: " << size << std::endl;

    *_f_bvec = new fuse_bufvec[sizeof(struct fuse_bufvec) + 1 * sizeof(struct fuse_buf)]();
    auto f_bvec = *_f_bvec;

    // NOTE: number of buffers
    f_bvec->count = 1;
    f_bvec->idx = 0;
    f_bvec->off = 0;

    f_bvec->buf->size = size;
    f_bvec->buf->mem = new char[size]();

    char* buf = (char*)f_bvec->buf->mem;

    int file_size = metadata_entry.file_size.value();
    int starting_fp = offset;
    int cur_fp = starting_fp;
    int buf_size = size;

    while(cur_fp - starting_fp < buf_size) {
      int block_order = cur_fp / BLOCK_SIZE;

      std::cout << "current fp: " << cur_fp << std::endl;
      std::cout << "reading block with order: " << block_order << std::endl;
      auto lookup_entry = FSLookupCollection::read_entry_with_inode_order(ffi.fh, block_order).value();
      std::cout << "reading block with fs data id: " << lookup_entry.fs_data_id << std::endl;
      auto data_entry = FSDataCollection::read_entry(lookup_entry.fs_data_id).value();

      // NOTE: start at current position within block based on FP
      for(int pos_in_block = cur_fp % BLOCK_SIZE; pos_in_block < BLOCK_SIZE;) {
        // NOTE: ensuring we don't overread in cur block
        if(cur_fp - starting_fp >= buf_size || cur_fp >= file_size) {
            return;
        }

        buf[cur_fp - starting_fp] = data_entry.buf[pos_in_block];

        // NOTE: updating pos and total bytes read
        cur_fp++;
        pos_in_block++;
      }
    }
}
