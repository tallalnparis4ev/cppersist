#ifndef MEM_CACHE_H_FILE
#define MEM_CACHE_H_FILE
#include "cache.hpp"
#include <map>
template <typename Ret, typename ...Args> 
class MemCache : public Cache<Ret,Args...>{
  public:
    const virtual std::map<std::string,std::string>& getContents() = 0;
};
#endif