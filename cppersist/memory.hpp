#ifndef MEM_H_FILE
#define MEM_H_FILE

#include <string>

#include "cacheimpl/memory/lrucache.hpp"
#include "cacheimpl/memory/onecache.hpp"
#include "cacheimpl/memory/regcache.hpp"
#include "memoization/memcachetypes.hpp"
#include "memoization/memoizable.hpp"
#include "memoization/memory/memoizer.hpp"

namespace cpst {
/**
 * Creates an object that has non-persistent memoization applied
 * to T's solve function. 
 * @tparam T - the class which the user wants to instantiate an object of,
 * with a memoized version of the solve function.
 * @param type - this will indicate the eviction strategy of the in-memory cache. Default is REGULAR.
 * @param size - maximum size of the cache, not applicable if type == ONE. Default size is 1000.
 * @param key - this function should return a string representation for a
 * given argument to the solve function.
 * @param pickle - this function should return a string representation
 * for any return of the solve function.
 * @param unpickle - this function is the inverse of pickle.
 */
template <typename T, typename Ret, typename... Args>
Memoized<T, Ret, Args...> getMemoizedObj(
    std::string (*key)(Args...), std::string (*pickle)(Ret),
    Ret (*unpickle)(std::string), MemCacheType type = MemCacheType::REGULAR,
    int size = 1000) {
  static_assert(std::is_base_of<Memoizable<Ret, Args...>, T>::value,
                "Must Memoize a class that inherits from Memoizable");
  switch (type) {
    case MemCacheType::ONE:
      return Memoized<T, Ret, Args...>(
          new OneCache<Ret, Args...>(key, pickle, unpickle));
    case MemCacheType::REGULAR:
      return Memoized<T, Ret, Args...>(
          new RegCache<Ret, Args...>(size, key, pickle, unpickle));
    case MemCacheType::LRU:
      return Memoized<T, Ret, Args...>(
          new LRUCache<Ret, Args...>(size, key, pickle, unpickle));
  }
  return Memoized<T, Ret, Args...>(
          new RegCache<Ret, Args...>(size, key, pickle, unpickle));
}

/**
 * Creates an object that has non-persistent memoization applied
 * to T's solve function. Will use a cache with eviction type REGULAR.
 * @tparam T - the class which the user wants to instantiate an object of,
 * with a memoized version of the solve function.
 * @param size - maximum size of the cache, not applicable if type == ONE. Default size is 1000.
 * @param key - this function should return a string representation for a
 * given argument to the solve function.
 * @param pickle - this function should return a string representation
 * for any return of the solve function.
 * @param unpickle - this function is the inverse of pickle.
 */
template <typename T, typename Ret, typename... Args>
Memoized<T, Ret, Args...> getMemoizedObj(std::string (*key)(Args...),
                                         std::string (*pickle)(Ret),
                                         Ret (*unpickle)(std::string),
                                         int size) {
  return getMemoizedObj<T, Ret, Args...>(key, pickle, unpickle,
                                         MemCacheType::REGULAR, size);
}
}  // namespace cpst
#endif
