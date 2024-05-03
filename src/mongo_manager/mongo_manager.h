#pragma once

#include <mongocxx/instance.hpp>

namespace MongoFuseFS {
  class MongoConnection {
    public:
    private:
      static const mongocxx::instance inst;
  };
}
