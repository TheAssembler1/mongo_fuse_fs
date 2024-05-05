#pragma once

#include <optional>
#include "md_entry.h"


#define FS_BIN_KEY "bin"

// NOTE: for file entries =====================================
// holds blocks of data for files
#define FILE_COLLECTION_NAME "fs_file"
#define FS_ID_KEY "fs_id"

typedef int FS_ID;

namespace mongo {

  class FSEntry {
    public:
      FSEntry(MDEntry md_entry, off_t offset, size_t size, char* buf) : md_entry{md_entry}, offset{offset}, size{size}, buf{buf} {}
      std::optional<int> create_entry();
      static std::optional<char*> read_entry_data(FS_ID fs_id);
    private:
      const off_t offset;
      const size_t size;
      const char* buf;
      MDEntry md_entry;
  };

}
