#include "manager.h"

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
