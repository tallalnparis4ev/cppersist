#ifndef ONE_CACHE_H_FILE
#define ONE_CACHE_H_FILE

#include "../../interfaces/memcache.hpp"
#include <optional>
#include <string>
namespace cpst{
  template <typename Ret, typename ...Args> 
  class OneCache : public MemCache<Ret,Args...>{
    public:
      OneCache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string));
      OneCache* clone();
      std::optional<Ret> get(const Args&... args);
      void put(const Args&... args, const Ret& value);
      void put(const std::string&, const std::string&);
      void populateCache(Cache<Ret,Args...>*);
    private:
      std::optional<string> getFromCache(const string&);
      std::string lastKey;
      std::string lastValue;      
  };
}
#include "onecache.cpp"
#endif