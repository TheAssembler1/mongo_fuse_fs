#pragma once

#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>

#define DB_URI "mongodb://localhost:27017"

// NOTE: for metadata entries =================================
// holds entries to metadata 
#define MD_COLLECTION_NAME "fs_metadata"
// the id of the root directory document in the metadata collection
#define MD_DOC_PATH_KEY "path"
#define MD_ROOT_DIR_DOC_PATH_VALUE "/"
// specifies whether metadata entries is a file or dir
#define MD_TYPE_KEY "type"
#define MD_FS_DIR_MD_TYPE "dir"
#define MD_FS_FILE_MD_TYPE "file"

// NOTE: for database =========================================
// the name of the database used to store all fs data
#define ROOT_DB_NAME "fs_root"


// NOTE: for file entries =====================================
// holds blocks of data for files
#define FILE_COLLECTION_NAME "fs_file"

namespace mongo_fuse_fs {

  namespace mongo_manager {
    void init_db();
    const inline mongocxx::instance inst;


    class MongoConnection {
      public:
        MongoConnection() : uri{DB_URI}, client{uri} {}
        const mongocxx::client& get_conn() const { return client; };
      private:
        const mongocxx::uri uri;
        const mongocxx::client client;
    };
  }

}
