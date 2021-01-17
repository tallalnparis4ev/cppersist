#ifndef MONGO_CACHE_H_FILE
#define MONGO_CACHE_H_FILE

#include "../../interfaces/percache.hpp"
#include <optional>
#include <string>
using std::string;
namespace cpst{
  /**
   * This is a class that represents a persistent cache which is stored in
   * a MongoDB database. 
   */
  template <typename Ret, typename ...Args> 
  class MongoDBCache : public PerCache<Ret,Args...>{
    public:
      MongoDBCache<Ret,Args...>* clone();
      MongoDBCache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string, string);
      MongoDBCache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string);
      std::optional<Ret> get(const Args&... args);
      void put(const Args&... args, const Ret& value);
    private:
      string base;
      string makeUrlForKey(const string& key);
  };
}
#include "mongodbcache.cpp"
#endif