#pragma once

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>

#include "../fs/perm.h"

#define DB_URI "mongodb://localhost:27017"

#define ROOT_DB_NAME "fs_root"
#define GET_ROOT_DB(conn) (conn)->get_client()[ROOT_DB_NAME] 
#define GET_MD_COLLECTION(conn) GET_ROOT_DB(conn)[MD_COLLECTION_NAME]
#define GET_FILE_COLLECTION(conn) GET_ROOT_DB(conn)[FILE_COLLECTION_NAME]
#define GET_FS_BLOCKS_COLLECTION(conn) GET_ROOT_DB(conn)[FS_BLOCKS_COLLECTION_NAME]

namespace mongo {

  class Connection {
    public:
      Connection() : uri{DB_URI}, client{uri} {}
      const mongocxx::client& get_client() const { return client; };
      
    private:
      const mongocxx::uri uri;
      const mongocxx::client client;
  };

}
