#include "cacheimpl/persistent/mongodbcache.hpp"
#include "cacheimpl/memory/regcache.hpp"
#include "cacheimpl/memory/lrucache.hpp"
#include "utils/log.hpp"
#include <iostream>
#include <string>

using std::string;
namespace cpst{
  template<typename T, typename Ret, typename... Args>
  PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string dbURL){
    static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
      "Must Memoize a class that inherits from PersistentMemoizable");
    
    string funcName = typeid(T).name();
    logOne("ALERT: No function name passed, using " + funcName + " as the function name instead!");
    return getMongoMemoizedObj<T,Ret,Args...>(key,pickle,unpickle,dbURL,funcName);
  }

  template<typename T, typename Ret, typename... Args>
  PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string dbURL, string funcName){
    static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
      "Must Memoize a class that inherits from PersistentMemoizable");
    
    MongoDBCache<Ret,Args...>* mongoCache = new MongoDBCache<Ret,Args...>(key,pickle,unpickle,dbURL,funcName);
    PersistentMemoized<T,Ret,Args...> memoized(mongoCache);
    return memoized;
  }

  template<typename T, typename Ret, typename... Args>
  PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(MemCacheType type,string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string dbURL){
    static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
      "Must Memoize a class that inherits from PersistentMemoizable");
    
    string funcName = typeid(T).name();
    logOne("ALERT: No function name passed, using " + funcName + " as the function name instead!");
    return getMongoMemoizedObj<T,Ret,Args...>(type,key,pickle,unpickle,dbURL,funcName);
  }

  template<typename T, typename Ret, typename... Args>
  PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(MemCacheType type,string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string dbURL, string funcName){
    static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
      "Must Memoize a class that inherits from PersistentMemoizable");

    MongoDBCache<Ret,Args...>* mongoCache = new MongoDBCache<Ret,Args...>(key,pickle,unpickle,dbURL,funcName);
    PersistentMemoized<T,Ret,Args...> memoized(type,mongoCache);
    return memoized;
  }
}