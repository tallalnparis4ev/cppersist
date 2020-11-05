#include <fstream>
#include <sstream>
#include <string>
#include <optional>
using std::optional;
using std::nullopt;
using std::function;
using std::string;
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

template <typename Ret, typename ...Args>
MongoDBCache<Ret,Args...>::MongoDBCache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string)){
  this->key = key;
  this->pickle = pickle;
  this->unpickle = unpickle;
  this->client = {mongocxx::uri{}};
  this->db = this->client["cppersist"];
  this->coll = this->db["results"];
}

template <typename Ret, typename ...Args> 
std::optional<Ret> MongoDBCache<Ret,Args...>::get(Args... args) {
  mongocxx::options::find opts{};
  opts.projection(document{} << "return" << 1 << finalize); //only care about the return value
  
  bsoncxx::stdx::optional<bsoncxx::document::value> maybeResult =
  coll.find_one(document{} << "_id" << this->key(args...) << finalize);

  if(maybeResult) {
    bsoncxx::document::view resultView = maybeResult->view();
    std::string ret = resultView["return"].get_utf8().value.to_string();
    return optional<Ret>{this->unpickle(ret)}; //try catch this
  }

  return nullopt; //not found in DB
}

template <typename Ret, typename ...Args> 
void MongoDBCache<Ret,Args...>::put(Args... args, Ret value) {
  std::cout << "keying for put: " << std::endl;
  bsoncxx::builder::stream::document builder = bsoncxx::builder::stream::document{};
  bsoncxx::document::view_or_value doc = builder
    << "_id" << this->key(args...)
    << "return" << this->pickle(value)
    << finalize;
  bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
  this->coll.insert_one(doc); //try catch this
}

// void insert(){
// }

// void find(){

// }