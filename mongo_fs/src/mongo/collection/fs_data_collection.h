#pragma once

#include <optional>
#include "fs_metadata_collection.h"

#define FS_BIN_KEY "bin"
#define FS_ID_KEY "fs_id"

typedef int FS_ID;

namespace mongo {
  class FSDataCollection {
    public:
      FSDataCollection(FSMetadataCollectionEntry md_entry, off_t offset, size_t size, char* buf) : md_entry{md_entry}, offset{offset}, size{size}, buf{buf} {}

      static constexpr std::string_view NAME = "fs_data";

      std::optional<int> create_entry();
      static std::optional<char*> read_entry_data(FS_ID fs_id);
    private:
      const off_t offset;
      const size_t size;
      const char* buf;
      FSMetadataCollectionEntry md_entry;
  };
}
