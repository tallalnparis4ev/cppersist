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
template <typename Ret, typename ...Args> 
class LRUCache : public MemCache<Ret,Args...>{
  public:
    LRUCache(int, string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&));
    std::optional<Ret> get(const Args&... args);
    void put(const Args&... args, const Ret& value);
    void put(const std::string&, const std::string&);
    void populateCache(Cache<Ret,Args...>*);
    void print();
  private:
    list<string> recentlyUsed;
    unordered_map<string, list<string>::iterator> positionCache;
    unordered_map<string, string> cache;
    int cacheSize; 
    std::optional<string> getFromCache(const string&);
};

#include "lrucache.cpp"
#endif