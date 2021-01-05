#include "cacheimpl/mongodbcache.hpp"
#include <iostream>
#include <string>

using std::string;

template<typename T, typename Ret, typename... Args>
PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&), string dbURL){
  static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
    "Must Memoize a class that inherits from PersistentMemoizable");
  
  string funcName = typeid(T).name();
  std::cout << "ALERT: No function name passed, using " << funcName << " as the function name instead!" << std::endl;
  MongoDBCache<Ret,Args...>* mongoCache = new MongoDBCache<Ret,Args...>(key,pickle,unpickle,dbURL,funcName);
  PersistentMemoized<T,Ret,Args...> memoized(mongoCache);
  return memoized;
}

template<typename T, typename Ret, typename... Args>
PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&), string dbURL, string funcName){
  static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
    "Must Memoize a class that inherits from PersistentMemoizable");
  
  MongoDBCache<Ret,Args...>* mongoCache = new MongoDBCache<Ret,Args...>(key,pickle,unpickle,dbURL,funcName);
  PersistentMemoized<T,Ret,Args...> memoized(mongoCache);
  return memoized;
}