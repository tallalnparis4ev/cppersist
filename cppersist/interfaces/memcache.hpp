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
    protected:
      virtual std::optional<std::string> getFromCache(const std::string&) = 0;
  };
}
#endif