#ifndef MONGO_CACHE_H_FILE
#define MONGO_CACHE_H_FILE

#include <optional>
#include <string>

#include "../../interfaces/percache.hpp"
using std::string;
namespace cpst {
/**
 * This is a class that represents a persistent cache which is stored in
 * a MongoDB database.
 */
template <typename Ret, typename... Args>
class MongoDBCache : public PerCache<Ret, Args...> {
 public:
  using PerCache<Ret, Args...>::PerCache;
  MongoDBCache(string (*key)(Args...), string (*pickle)(Ret),
               Ret (*unpickle)(string), string (*hash)(string), string, string);
  MongoDBCache<Ret, Args...>* clone();
  std::optional<Ret> get(const Args&... args);
  void put(const Args&... args, const Ret& value);
  void setLoc(string&);

 private:
  static const string NAMESPACE;
  static const string COLLECTION_NAME;
  MongoDBCache(string (*key)(Args...), string (*pickle)(Ret),
               Ret (*unpickle)(string), string (*hash)(string), string);
  string base;
  string makeUrlForKey(const string& key);
};
}  // namespace cpst
#include "mongodbcache.cpp"
#endif