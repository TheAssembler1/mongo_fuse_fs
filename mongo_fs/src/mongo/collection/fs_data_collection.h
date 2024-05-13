#ifndef __FS_DATA_COLLECTION_H__
#define __FS_DATA_COLLECTION_H__

#include <mongocxx/client.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <vector>
#include <cassert>
#include <optional>

#include "../../fs/fs_config.h"
#include "../connection.h"
#include "types.h"

using bsoncxx::document::value;
using bsoncxx::types::b_binary;
using bsoncxx::binary_sub_type;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace mongo {
  class FSDataCollectionEntry {
    public:
      FSDataCollectionEntry (const FSDataCollectionEntry&) = delete;
      FSDataCollectionEntry& operator= (const FSDataCollectionEntry&) = delete;
      FSDataCollectionEntry(FS_DATA_ID data_id, char* _buf) : fs_data_id{data_id}, buf{_buf} {}
      FSDataCollectionEntry(FSDataCollectionEntry&& entry) {
        fs_data_id = entry.fs_data_id;
        buf = entry.buf;
        entry.buf = nullptr;
      }
      FSDataCollectionEntry() : buf{new char[fs::FSConfig::BLOCK_SIZE]()} {}

      static FSDataCollectionEntry bson_to_entry(value bson_doc);

      static constexpr std::string_view FS_DATA_ID_KEY = "_id";
      static constexpr std::string_view FS_BUF_KEY = "buf";

      std::optional<FS_DATA_ID> fs_data_id = std::nullopt;

      ~FSDataCollectionEntry() {
        delete buf;
      }

      // NOTE: buffer of BLOCK_SIZE data
      char* buf;
  };


  class FSDataCollection {
    public:
      static std::optional<FS_DATA_ID> create_entry(FSDataCollectionEntry& fs_data_collection_entry);
      static std::optional<FSDataCollectionEntry> read_entry(FS_DATA_ID fs_data_id);
      static void update_entry(FSDataCollectionEntry& fs_data_collection_entry);

      static constexpr std::string_view NAME = "fs_data";
  };
}

#endif
