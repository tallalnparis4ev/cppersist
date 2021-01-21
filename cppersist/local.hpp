#ifndef LOCAL_H_FILE
#define LOCAL_H_FILE

#include "memoization/persister.hpp"
#include "crypto/sha256.h"

namespace cpst{
  /**
   * \defgroup PersistLocal Persist Local Filesystem
   * @{
   */

  /**
   * Creates an object, of type T, that has persistent memoization applied 
   * to T's solve function. Here, the storage medium for persistent memoization
   * is the local file system.
   * @tparam T - the class which the user wants to instantiate an object of,
   * with a memoized version of the solve function.
   * @param key - this function should return a string representation for a 
   * given argument to the solve function. 
   * @param pickle - this function should return a string representation
   * for any return of the solve function.
   * @param unpickle - this function is the inverse of pickle.
   */ 
  template<typename T, typename Ret, typename ...Args>
  PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string (*hash)(string) = sha256);

  /**
   * Creates an object, of type T, that has persistent memoization applied 
   * to T's solve function. Here, the storage medium for persistent memoization
   * is the local file system. 
   * @tparam T - the class which the user wants to instantiate an object of,
   * with a memoized version of the solve function.
   * @param key - this function should return a string representation for a 
   * given argument to the solve function. 
   * @param pickle - this function should return a string representation
   * for any return of the solve function.
   * @param unpickle - this function is the inverse of pickle.
   * @param funcName - files will be stored at "./cppersist/{funcName}", if 
   * funcName is not specified typeid(T).name() is used instead.
   */ 
  template<typename T, typename Ret, typename... Args>
  PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string funcName, string (*hash)(string) = sha256);

  /**
   * Creates an object, of type T, that has persistent memoization applied 
   * to T's solve function. Here, the storage medium for persistent memoization
   * is the local file system. 
   * @tparam T - the class which the user wants to instantiate an object of,
   * with a memoized version of the solve function.
   * @param primaryCache - this will initialise the specified memory cache and use
   * it as the primary cache.
   * @param key - this function should return a string representation for a 
   * given argument to the solve function. 
   * @param pickle - this function should return a string representation
   * for any return of the solve function.
   * @param unpickle - this function is the inverse of pickle.
   */ 
  template<typename T, typename Ret, typename... Args>
  PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(MemCacheType primaryCache,string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string (*hash)(string) = sha256);

  /**
   * Creates an object, of type T, that has persistent memoization applied 
   * to T's solve function. Here, the storage medium for persistent memoization
   * is the local file system. 
   * @tparam T - the class which the user wants to instantiate an object of,
   * with a memoized version of the solve function.
   * @param primaryCache - this will initialise the specified memory cache and use
   * it as the primary cache.
   * @param key - this function should return a string representation for a 
   * given argument to the solve function. 
   * @param pickle - this function should return a string representation
   * for any return of the solve function.
   * @param unpickle - this function is the inverse of pickle.
   * @param funcName - files will be stored at "./cppersist/{funcName}", if 
   * funcName is not specified typeid(T).name() is used instead.
   */ 
  template<typename T, typename Ret, typename... Args>
  PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(MemCacheType primaryCache,string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string funcName, string (*hash)(string) = sha256);

  /**@}*/
}
#include "local.cpp"
#endif