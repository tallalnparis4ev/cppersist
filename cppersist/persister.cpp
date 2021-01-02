#include <optional>
#include <string>
#include <functional>
#include <iostream>
#include "cache.hpp"
#include "diskcache.hpp"
#include "mongodbcache.hpp"
using std::function;
using std::string;

template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...>::PersistentMemoized(Cache<Ret,Args...>* cache){
  this->persistentCache = cache;
}

template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...>::~PersistentMemoized(){
  free(this->persistentCache);
}

template<typename T, typename Ret, typename ...Args>
Ret PersistentMemoized<T,Ret,Args...>::operator()(Args... args) {
  return solve(args...);
}

template<typename T, typename Ret, typename ...Args>
Ret PersistentMemoized<T,Ret,Args...>::solve(Args... args){
  optional<Ret> answer = persistentCache->get(args...);  
  if(answer){
    std::cout << "CACHE HIT" << std::endl;
    return answer.value();
  }
  Ret realAnswer = T::solve(args...);
  persistentCache->put(args..., realAnswer);
  return realAnswer;  
}

template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string)){
  static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
    "Must Memoize a class that inherits from PersistentMemoizable");

  string funcName = typeid(T).name();
  std::cout << "ALERT: No function name passed, using " << funcName << " as the function name instead!" << std::endl;
  DiskCache<Ret,Args...>* diskCache = new DiskCache<Ret,Args...>(key,pickle,unpickle,funcName);
  PersistentMemoized<T,Ret,Args...> memoized(diskCache);
  return memoized;
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
PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string dbURL){
  static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
    "Must Memoize a class that inherits from PersistentMemoizable");
  
  string funcName = typeid(T).name();
  std::cout << "ALERT: No function name passed, using " << funcName << " as the function name instead!" << std::endl;
  MongoDBCache<Ret,Args...>* mongoCache = new MongoDBCache<Ret,Args...>(key,pickle,unpickle,dbURL,funcName);
  PersistentMemoized<T,Ret,Args...> memoized(mongoCache);
  return memoized;
}

template<typename T, typename Ret, typename... Args>
PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string dbURL, string funcName){
  static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
    "Must Memoize a class that inherits from PersistentMemoizable");
  
  MongoDBCache<Ret,Args...>* mongoCache = new MongoDBCache<Ret,Args...>(key,pickle,unpickle,dbURL,funcName);
  PersistentMemoized<T,Ret,Args...> memoized(mongoCache);
  return memoized;
}
