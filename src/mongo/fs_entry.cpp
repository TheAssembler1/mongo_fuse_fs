#include "fs_entry.h"
#include "connection.h"
#include "manager.h"
#include <mongocxx/client.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <vector>
#include <cassert>

using namespace mongo;

using bsoncxx::types::b_binary;
using bsoncxx::binary_sub_type;
using bsoncxx::binary_sub_type;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

std::optional<int> FSEntry::create_entry() {
  Connection conn;
  auto file_collection = GET_FILE_COLLECTION(&conn);
  std::cout << "writing to fd: " << md_entry.fd << std::endl;

  std::streamsize stream_size = size;
  b_binary data{binary_sub_type::k_binary, static_cast<std::uint32_t>(stream_size), reinterpret_cast<const std::uint8_t*>(buf)};
  
  std::cout << "writing data to fd_collection" << std::endl;
  int fs_id = Manager::generate_id();
  auto result_doc =  file_collection.insert_one(make_document(
    kvp(FS_ID_KEY, fs_id),
    kvp(FS_BIN_KEY, data)
  ));

  assert(result_doc);

  auto query = file_collection.find_one(make_document(
    kvp(FS_ID_KEY, fs_id)
  ));
  
  const char* binary = (const char*)query.value()[FS_BIN_KEY].get_binary().bytes;
  std::cout << "output binary: " << binary << std::endl;

  return 123;
}