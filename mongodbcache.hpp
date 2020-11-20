#include "cache.hpp"
#include <optional>
#include <string>
using std::string;

template <typename Ret, typename ...Args> 
class MongoDBCache : public Cache<Ret,Args...>{
  public:
    MongoDBCache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string, string);
    std::optional<Ret> get(Args... args);
    void put(Args... args, Ret value);
  private:
    string base;
    string makeUrlForKey(string key);
};

#if !defined(MONGODBCACHE_H_FILE)
#define MONGODBCACHE_H_FILE
#include "mongodbcache.cpp"
#endif
