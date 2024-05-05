// NOTE: needed to establish fuse version early
#define FUSE_USE_VERSION 35
#include <fuse.h>
#include <iostream>
#include <assert.h>
#include <random>
#include <bsoncxx/builder/stream/helpers.hpp>

#include "manager.h"
#include "../fs/perm.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::array;
using bsoncxx::builder::basic::document;

using namespace mongo;

void Manager::init_db() {
  std::cout << "init database" << std::endl;
}

int Manager::generate_id() {
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<int> dis;
  
  return dis(gen);
}
