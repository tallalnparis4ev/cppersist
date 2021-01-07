#ifndef MONGO_CACHE_H_FILE
#define MONGO_CACHE_H_FILE

#include "../../interfaces/percache.hpp"
#include <optional>
#include <string>
using std::string;

template <typename Ret, typename ...Args> 
class MongoDBCache : public PerCache<Ret,Args...>{
  public:
    MongoDBCache<Ret,Args...>* clone();
    MongoDBCache(string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&), string, string);
    MongoDBCache(string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&), string);
    std::optional<Ret> get(const Args&... args);
    void put(const Args&... args, const Ret& value);
    void put(const std::string& key, const std::string& value);
  private:
    string base;
    string makeUrlForKey(const string& key);
};

#include "mongodbcache.cpp"
#endif