#include <iostream>
#include <assert.h>
#include "mongo_manager.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

using namespace mongo_fuse_fs;

void mongo_manager::init_db() {
  std::cout << "creating root database" << std::endl;
  
  std::cout << "getting mongo connection for db init" << std::endl;
  MongoConnection mongo_conn;
  auto& conn =  mongo_conn.get_conn();
  
  std::cout << "creating or ensuring creation of root db" << std::endl;
  auto root_db = conn[ROOT_DB_NAME];

  std::cout << "creating or ensuring creation of metadata and file collection" << std::endl;
  auto file_collection = root_db[FILE_COLLECTION_NAME];
  auto md_collection = root_db[MD_COLLECTION_NAME];

  std::cout << "creating root dir doc" << std::endl;
  auto root_dir_doc = md_collection.insert_one(make_document(
      kvp(MD_DOC_PATH_KEY, MD_ROOT_DIR_DOC_PATH_VALUE),
      kvp(MD_TYPE_KEY, MD_FS_DIR_MD_TYPE)
  ));
  
  assert(root_dir_doc);
  auto root_doc_id = root_dir_doc->inserted_id();

}
