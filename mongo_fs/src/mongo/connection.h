#pragma once

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>

#include "collection/fs_metadata_collection.h"
#include "collection/fs_data_collection.h"
#include "collection/fs_lookup_collection.h"
#include "../fs/perm.h"

#define DB_URI "mongodb://localhost:27017"

#define ROOT_DB_NAME "fs_root"
#define GET_ROOT_DB(conn) (conn)->get_client()[ROOT_DB_NAME] 
#define GET_FS_METADATA_COLLECTION(conn) GET_ROOT_DB(conn)[mongo::FSMetadataCollection::NAME]
#define GET_FS_LOOKUP_COLLECTION(conn) GET_ROOT_DB(conn)[mongo::FSLookupCollection::NAME]
#define GET_FS_DATA_COLLECTION(conn) GET_ROOT_DB(conn)[mongo::FSDataCollection::NAME]

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
