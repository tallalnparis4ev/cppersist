#include <iostream>
#include <string>
#include <optional>
#include <cpr/cpr.h>
using std::optional;
using std::nullopt;
using std::function;
using std::string;

namespace cpst{
  template <typename Ret, typename ...Args> 
  MongoDBCache<Ret,Args...>* MongoDBCache<Ret,Args...>::clone(){
    return new MongoDBCache<Ret,Args...>(this->key,this->pickle,this->unpickle,this->base);
  }

  template <typename Ret, typename ...Args>
  MongoDBCache<Ret,Args...>::MongoDBCache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string),
  string base){
    this->key = key;
    this->pickle = pickle;
    this->unpickle = unpickle;
    this->base = base;
  }

  template <typename Ret, typename ...Args>
  MongoDBCache<Ret,Args...>::MongoDBCache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string),
  string dbURL, string funcName) : MongoDBCache(key,pickle,unpickle,dbURL + "/" + funcName + "/") {}

  template <typename Ret, typename ...Args> 
  string MongoDBCache<Ret,Args...>::makeUrlForKey(const string& key) {
    return this->base + key;
  }

  template <typename Ret, typename ...Args> 
  std::optional<Ret> MongoDBCache<Ret,Args...>::get(const Args&... args) {
    cpr::Response response = cpr::Get(cpr::Url{makeUrlForKey(this->key(args...))}); //catch error
    if(response.status_code == 200){
      return optional<Ret>{this->unpickle(response.text)};
    }
    return nullopt;
  }

  template <typename Ret, typename ...Args> 
  void MongoDBCache<Ret,Args...>::put(const Args&... args, const Ret& value) {
    string key = this->key(args...);
    string valueStr = this->pickle(value);
    put(key,valueStr);
  }
}