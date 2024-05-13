#ifndef __COLLECTION_HELPER_H__
#define __COLLECTION_HELPER_H__

// NOTE: needed to establish fuse version early
#define FUSE_USE_VERSION 35
#include <cassert>
#include <fuse.h>
#include <iostream>

#include "../mongo_manager.h"
#include "fs_data_collection.h"
#include "fs_lookup_collection.h"
#include "fs_metadata_collection.h"
#include "types.h"

namespace mongo {

class CollectionHelper {
    public:
    static void create_next_data_lookup_entries(INODE inode);
    static int write_fuse_bufvec_to_mongo(const char* path, fuse_bufvec& f_bvec, off_t offset, fuse_file_info& ffi);
    static void read_mongo_to_fuse_bufvec(const char* path, fuse_bufvec** _f_bvec, size_t size, off_t offset, fuse_file_info& ffi);
    static void print_all_file(INODE inode);
    static void truncate_file(INODE inode, off_t new_size);
};

} // namespace mongo

#endif
