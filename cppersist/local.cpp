#include <iostream>
#include <string>

#include "cacheimpl/persistent/diskcache.hpp"
#include "crypto/sha256.h"
#include "memoization/memoizable.hpp"
#include "utils/log.hpp"

using std::string;
namespace cpst {
// Get a disk memoized function, using a default name for the collection
template <typename T, typename Ret, typename... Args>
PersistentMemoized<T, Ret, Args...> getLocalMemoizedObj(
    string (*key)(Args...), string (*pickle)(Ret), Ret (*unpickle)(string),
    string (*hash)(string)) {
  static_assert(std::is_base_of<Memoizable<Ret, Args...>, T>::value,
                "Must Memoize a class that inherits from Memoizable");

  string funcName = typeid(T).name();
  logOne("ALERT: No function name passed, using " + funcName +
         " as the function name instead!");
  return getLocalMemoizedObj<T, Ret, Args...>(key, pickle, unpickle, funcName,
                                              hash);
}

// Get a disk memoized function, using a user-specified name for the collection
template <typename T, typename Ret, typename... Args>
PersistentMemoized<T, Ret, Args...> getLocalMemoizedObj(
    string (*key)(Args...), string (*pickle)(Ret), Ret (*unpickle)(string),
    string funcName, string (*hash)(string)) {
  static_assert(std::is_base_of<Memoizable<Ret, Args...>, T>::value,
                "Must Memoize a class that inherits from Memoizable");

  DiskCache<Ret, Args...>* diskCache =
      new DiskCache<Ret, Args...>(key, pickle, unpickle, hash, funcName);
  return PersistentMemoized<T, Ret, Args...>(diskCache);
}

// Get a MongoDB memoized function, using a user-specifed name for the
// collection. Also, use an in-memory cache of type Type.
template <typename T, typename Ret, typename... Args>
PersistentMemoized<T, Ret, Args...> getLocalMemoizedObj(
    MemCacheType type, string (*key)(Args...), string (*pickle)(Ret),
    Ret (*unpickle)(string), string funcName, string (*hash)(string)) {
  static_assert(std::is_base_of<Memoizable<Ret, Args...>, T>::value,
                "Must Memoize a class that inherits from Memoizable");

  DiskCache<Ret, Args...>* diskCache =
      new DiskCache<Ret, Args...>(key, pickle, unpickle, hash, funcName);
  return PersistentMemoized<T, Ret, Args...>(type, diskCache);
}

// Get a MongoDB memoized function, using a default name for the collection.
// Also, use an in-memory cache of type Type.
template <typename T, typename Ret, typename... Args>
PersistentMemoized<T, Ret, Args...> getLocalMemoizedObj(
    MemCacheType type, string (*key)(Args...), string (*pickle)(Ret),
    Ret (*unpickle)(string), string (*hash)(string)) {
  static_assert(std::is_base_of<Memoizable<Ret, Args...>, T>::value,
                "Must Memoize a class that inherits from Memoizable");

  string funcName = typeid(T).name();
  logOne("ALERT: No function name passed, using " + funcName +
         " as the function name instead!");
  return getLocalMemoizedObj<T, Ret, Args...>(type, key, pickle, unpickle,
                                              funcName, hash);
}
}  // namespace cpst