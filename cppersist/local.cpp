#include "cacheimpl/persistent/diskcache.hpp"
#include "utils/log.hpp"
#include <iostream>
#include <string>

using std::string;
namespace cpst{
  template<typename T, typename Ret, typename ...Args>
  PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string)){
    static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
      "Must Memoize a class that inherits from PersistentMemoizable");

    string funcName = typeid(T).name();
    logOne("ALERT: No function name passed, using " + funcName + " as the function name instead!");
    return getLocalMemoizedObj<T,Ret,Args...>(key,pickle,unpickle,funcName);
  }

  template<typename T, typename Ret, typename... Args>
  PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string funcName){
    static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
      "Must Memoize a class that inherits from PersistentMemoizable");

    DiskCache<Ret,Args...>* diskCache = new DiskCache<Ret,Args...>(key,pickle,unpickle,funcName);
    PersistentMemoized<T,Ret,Args...> memoized(diskCache);
    return memoized;
  }

  template<typename T, typename Ret, typename... Args>
  PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(MemCacheType type,string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string funcName){
    static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
      "Must Memoize a class that inherits from PersistentMemoizable");
    
    DiskCache<Ret,Args...>* diskCache = new DiskCache<Ret,Args...>(key,pickle,unpickle,funcName);
    PersistentMemoized<T,Ret,Args...> memoized(type,diskCache);
    return memoized;
  }

  template<typename T, typename Ret, typename... Args>
  PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(MemCacheType type,string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string)){
    static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
      "Must Memoize a class that inherits from PersistentMemoizable");
    
    string funcName = typeid(T).name();
    logOne("ALERT: No function name passed, using " + funcName + " as the function name instead!");
    return getLocalMemoizedObj<T,Ret,Args...>(type,key,pickle,unpickle,funcName);
  }
}