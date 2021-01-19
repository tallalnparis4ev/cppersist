#ifndef ONE_CACHE_H_FILE
#define ONE_CACHE_H_FILE

#include "../../interfaces/memcache.hpp"
#include <optional>
#include <string>
namespace cpst{
  /**
   * This is a class that represents an in-memory cache which only stores 
   * the most recent key/value pair. 
   */
  template <typename Ret, typename ...Args> 
  class OneCache : public MemCache<Ret,Args...>{
    public:
      OneCache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string));
      OneCache* clone();
      std::optional<Ret> get(const Args&... args);
      void put(const Args&... args, const Ret& value);
    private:
      std::optional<string> getFromCache(const string&);
      std::string lastKey;
      std::string lastValue;      
  };
}
#include "onecache.cpp"
#endif