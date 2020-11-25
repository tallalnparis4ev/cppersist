#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <optional>
#include <cpr/cpr.h>
using std::optional;
using std::nullopt;
using std::function;
using std::string;



template <typename Ret, typename ...Args>
MongoDBCache<Ret,Args...>::MongoDBCache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string),
string dbURL, string funcName){
  this->key = key;
  this->pickle = pickle;
  this->unpickle = unpickle;
  this->funcName = funcName;
  this->base = dbURL + "/" + funcName + "/";
}

template <typename Ret, typename ...Args> 
string MongoDBCache<Ret,Args...>::makeUrlForKey(string key) {
  return this->base + key;
}

template <typename Ret, typename ...Args> 
std::optional<Ret> MongoDBCache<Ret,Args...>::get(Args... args) {
  cpr::Response response = cpr::Get(cpr::Url{makeUrlForKey(this->key(args...))}); //catch error
  if(response.status_code == 200){
    return optional<Ret>{this->unpickle(response.text)};
  }
  return nullopt;
}

template <typename Ret, typename ...Args> 
void MongoDBCache<Ret,Args...>::put(Args... args, Ret value) {
  cpr::Response response = cpr::Post(cpr::Url{makeUrlForKey(this->key(args...))},
                   cpr::Body{"{\"return\": \"" +this->pickle(value)+"\"}"},
                   cpr::Header{{"Content-Type", "application/json"}});
}