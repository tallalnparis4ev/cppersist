#ifndef REG_CACHE_H_FILE
#define REG_CACHE_H_FILE

#include "../../interfaces/memcache.hpp"
#include <optional>
#include <unordered_map>
#include <string>
namespace cpst{
  /**
   * This is a class that represents an in-memory cache with no bound on the
   * number of entries it has.
   */
  template <typename Ret, typename ...Args> 
  class RegCache : public MemCache<Ret,Args...>{
    public:
      RegCache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string));
      RegCache* clone();
      std::optional<Ret> get(const Args&... args);
      void put(const Args&... args, const Ret& value);
      void populateCache(Cache<Ret,Args...>*);
    private:
      std::optional<string> getFromCache(const string&);
      std::unordered_map<string,string> cache;
  };
}
#include "regcache.cpp"
#endif