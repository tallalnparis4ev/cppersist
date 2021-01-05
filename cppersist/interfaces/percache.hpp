#ifndef PER_CACHE_H_FILE
#define PER_CACHE_H_FILE
#include "cache.hpp"
template <typename Ret, typename ...Args> 
class PerCache : public Cache<Ret,Args...>{
  protected:
    std::string funcName;
};
#endif