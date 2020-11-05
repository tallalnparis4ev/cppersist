#include "cache.hpp"
#include <optional>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
template <typename Ret, typename ...Args> 
class MongoDBCache : public Cache<Ret,Args...>{
  public:
    MongoDBCache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string));
    std::optional<Ret> get(Args... args);
    void put(Args... args, Ret value);
    static mongocxx::instance instance;
  private:
    // mongocxx::instance instance; // Shouldn't be done twice
    mongocxx::client client;
    mongocxx::database db;
    mongocxx::collection coll;
};

#if !defined(MONGODBCACHE_H_FILE)
#define MONGODBCACHE_H_FILE
#include "mongodbcache.cpp"
#endif
