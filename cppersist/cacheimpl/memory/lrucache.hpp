#ifndef LRU_CACHE_H_FILE
#define LRU_CACHE_H_FILE

#include <list>
#include <optional>
#include <string>
#include <unordered_map>

#include "../../interfaces/memcache.hpp"
using std::list;
using std::string;
using std::unordered_map;
namespace cpst {
/**
 * This is a class that represents an in-memory cache which has a limit
 * on the number of entries it can store. When that limit is reached,
 * entries are removed from the cache using a least recently used policy.
 */
template <typename Ret, typename... Args>
class LRUCache : public MemCache<Ret, Args...> {
 public:
  LRUCache(int size, std::string (*key)(Args...), std::string (*pickle)(Ret),
           Ret (*unpickle)(std::string))
      : MemCache<Ret, Args...>(size, key, pickle, unpickle) {}
  LRUCache* clone();
  std::optional<Ret> get(const Args&... args);
  void put(const Args&... args, const Ret& value);

 private:
  list<string> recentlyUsed;
  unordered_map<string, list<string>::iterator> positionCache;
  unordered_map<string, string> cache;
  std::optional<string> getFromCache(const string&);
};
}  // namespace cpst
#include "lrucache.cpp"
#endif