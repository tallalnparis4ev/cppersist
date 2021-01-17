#ifndef MEM_CACHE_H_FILE
#define MEM_CACHE_H_FILE
#include "cache.hpp"
#include <string>
#include <optional>
namespace cpst{
  /**
   * This is an abstract class representing a cache that is stored 
   * in-memory. 
   */
  template <typename Ret, typename ...Args> 
  class MemCache : public Cache<Ret,Args...>{
    public:
      virtual void populateCache(Cache<Ret,Args...>*) = 0;
    protected:
      virtual std::optional<std::string> getFromCache(const std::string&) = 0;
  };
}
#endif