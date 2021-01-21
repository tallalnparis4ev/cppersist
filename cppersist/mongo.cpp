#include "cacheimpl/persistent/mongodbcache.hpp"
#include "cacheimpl/memory/regcache.hpp"
#include "cacheimpl/memory/lrucache.hpp"
#include "utils/log.hpp"
#include <iostream>
#include <string>

using std::string;
namespace cpst{
  //Get a MongoDB memoized function, using a default name for the collection
  template<typename T, typename Ret, typename... Args>
  PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string dbURL, string (*hash)(string)){
    static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
      "Must Memoize a class that inherits from PersistentMemoizable");
    
    string funcName = typeid(T).name();
    logOne("ALERT: No function name passed, using " + funcName + " as the function name instead!");
    return getMongoMemoizedObj<T,Ret,Args...>(key,pickle,unpickle,dbURL,funcName,hash);
  }

  //Get a MongoDB memoized function, using a user-specifed name for the collection
  template<typename T, typename Ret, typename... Args>
  PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string dbURL, string funcName, string (*hash)(string)){
    static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
      "Must Memoize a class that inherits from PersistentMemoizable");
    
    MongoDBCache<Ret,Args...>* mongoCache = new MongoDBCache<Ret,Args...>(key,pickle,unpickle,hash,dbURL,funcName);
    return PersistentMemoized<T,Ret,Args...>(mongoCache);
  }

  //Get a MongoDB memoized function, using a default name for the collection.
  //Also, use an in-memory cache of type Type.
  template<typename T, typename Ret, typename... Args>
  PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(MemCacheType type,string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string dbURL, string (*hash)(string)){
    static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
      "Must Memoize a class that inherits from PersistentMemoizable");
    
    string funcName = typeid(T).name();
    logOne("ALERT: No function name passed, using " + funcName + " as the function name instead!");
    return getMongoMemoizedObj<T,Ret,Args...>(type,key,pickle,unpickle,dbURL,funcName,hash);
  }

  //Get a MongoDB memoized function, using a user-specifed name for the collection.
  //Also, use an in-memory cache of type Type.
  template<typename T, typename Ret, typename... Args>
  PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(MemCacheType type,string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string dbURL, string funcName, string (*hash)(string)){
    static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
      "Must Memoize a class that inherits from PersistentMemoizable");

    MongoDBCache<Ret,Args...>* mongoCache = new MongoDBCache<Ret,Args...>(key,pickle,unpickle,hash,dbURL,funcName);
    return PersistentMemoized<T,Ret,Args...>(type,mongoCache);
  }
}