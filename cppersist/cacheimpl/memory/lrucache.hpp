#ifndef LRU_CACHE_H_FILE
#define LRU_CACHE_H_FILE

#include "../../interfaces/memcache.hpp"
#include <optional>
#include <unordered_map>
#include <list>
#include <string>
using std::string;
using std::list;
using std::unordered_map;
namespace cpst{
  /**
   * This is a class that represents an in-memory cache which has a limit
   * on the number of entries it can store. When that limit is reached,
   * entries are removed from the cache using a least recently used policy.
   */
  template <typename Ret, typename ...Args> 
  class LRUCache : public MemCache<Ret,Args...>{
    public:
      LRUCache(int, string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string));
      LRUCache* clone();
      std::optional<Ret> get(const Args&... args);
      void put(const Args&... args, const Ret& value);
      void populateCache(Cache<Ret,Args...>*);
    private:
      list<string> recentlyUsed;
      unordered_map<string, list<string>::iterator> positionCache;
      unordered_map<string, string> cache;
      int cacheSize; 
      std::optional<string> getFromCache(const string&);
  };
}
#include "lrucache.cpp"
#endif