#pragma once

#include <optional>
#include "md_entry.h"


#define FS_BIN_KEY "bin"

// NOTE: for file entries =====================================
// holds blocks of data for files
#define FILE_COLLECTION_NAME "fs_file"
#define FS_ID_KEY "_id"

namespace mongo {

  class FSEntry {
    public:
      FSEntry(MDEntry md_entry, off_t offset, size_t size, char* buf) : md_entry{md_entry}, offset{offset}, size{size}, buf{buf} {}
      std::optional<int> create_entry();
    private:
      const off_t offset;
      const size_t size;
      const char* buf;
      MDEntry md_entry;
  };

}
