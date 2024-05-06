#include <mongocxx/client.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <vector>
#include <cassert>
#include <optional>

#include "fs_data_collection.h"
#include "fs_lookup_collection.h"
#include "../connection.h"
#include "../manager.h"

using namespace mongo;

using bsoncxx::types::b_binary;
using bsoncxx::binary_sub_type;
using bsoncxx::binary_sub_type;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

std::optional<int> FSDataCollection::create_entry() {
  Connection conn;
  auto file_collection = GET_FS_DATA_COLLECTION(&conn);
  std::cout << "writing to fd: " << md_entry.inode << std::endl;

  std::streamsize stream_size = size;
  b_binary data{binary_sub_type::k_binary, static_cast<std::uint32_t>(stream_size), reinterpret_cast<const std::uint8_t*>(buf)};
  
  std::cout << "writing data to fd_collection" << std::endl;
  int fs_id = Manager::generate_id();
  auto result_doc =  file_collection.insert_one(make_document(
    kvp(FS_ID_KEY, fs_id),
    kvp(FS_BIN_KEY, data)
  ));

  assert(result_doc);

  if(!result_doc.has_value()) {
    return std::nullopt;
  }

  FSLookupCollection fs_lookup{md_entry.inode};
  auto res = fs_lookup.create_entry(fs_id); 

  if(res.has_value()) {
    std::cout << "create fs lookup entry with fs_id: " << res.value() << std::endl;
  } else {
    std::cout << "failed to create fs lookup entry" << std::endl; 
  }

  return size;
}

std::optional<char*> FSDataCollection::read_entry_data(FS_ID fs_id) {
  Connection conn;
  auto file_collection = GET_FS_DATA_COLLECTION(&conn);

  auto bson_res = file_collection.find_one(make_document(kvp(FS_ID_KEY, fs_id)));

  assert(bson_res);

  if(!bson_res.has_value()) {
    return std::nullopt;
  }

  char* buf = (char*)bson_res.value()[FS_BIN_KEY].get_binary().bytes;

  return buf;
}
