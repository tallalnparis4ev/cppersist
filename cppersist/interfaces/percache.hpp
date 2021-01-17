#ifndef PER_CACHE_H_FILE
#define PER_CACHE_H_FILE
#include "cache.hpp"
namespace cpst{
  /**
   * This is an abstract class representing a cache that is stored in 
   * persistent storage.
   */
  template <typename Ret, typename ...Args> 
  class PerCache : public Cache<Ret,Args...>{
    protected:
      std::string funcName;
  };
}
#endif