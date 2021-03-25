#ifndef MEM_H_FILE
#define MEM_H_FILE

#include "cacheimpl/memory/onecache.hpp"
#include "cacheimpl/memory/regcache.hpp"
#include "cacheimpl/memory/lrucache.hpp"
#include "memoization/memory/memoizer.hpp"
#include "memoization/memcachetypes.hpp"
#include "memoization/memoizable.hpp"
#include <string>

namespace cpst{
  template <typename T, typename Ret, typename... Args>
  Memoized<T,Ret,Args...> getMemoizedObj(std::string (*key)(Args...), std::string (*pickle)(Ret),
    Ret (*unpickle)(std::string), MemCacheType type = MemCacheType::REGULAR, int size = 1000){
    static_assert(std::is_base_of<Memoizable<Ret, Args...>, T>::value,
                "Must Memoize a class that inherits from Memoizable");
    std::string x = "";
    switch (type)
    {
    case MemCacheType::ONE: x = "one";
      break;
      case MemCacheType::REGULAR: x = "reg";
      break;
      case MemCacheType::LRU_CACHE: x = "lru";
      break;
    }            
    log(x,size);
    switch (type)
    {
    case MemCacheType::ONE:
        return Memoized<T,Ret,Args...>(new OneCache<Ret,Args...>(key,pickle,unpickle));
    case MemCacheType::REGULAR:
        return Memoized<T,Ret,Args...>(new RegCache<Ret,Args...>(key,pickle,unpickle));
    case MemCacheType::LRU_CACHE:
        return Memoized<T,Ret,Args...>(new LRUCache<Ret,Args...>(size,key,pickle,unpickle));
    }
  }

  template <typename T, typename Ret, typename... Args>
  Memoized<T,Ret,Args...> getMemoizedObj(std::string (*key)(Args...), std::string (*pickle)(Ret),
    Ret (*unpickle)(std::string), int size){
      logOne("aux");
    return getMemoizedObj<T,Ret,Args...>(key,pickle,unpickle,MemCacheType::REGULAR,size);
  }
}
#endif