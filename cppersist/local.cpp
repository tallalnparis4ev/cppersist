#include "diskcache.hpp"
#include <iostream>
#include <string>

using std::string;

template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&)){
  static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
    "Must Memoize a class that inherits from PersistentMemoizable");

  string funcName = typeid(T).name();
  std::cout << "ALERT: No function name passed, using " << funcName << " as the function name instead!" << std::endl;
  DiskCache<Ret,Args...>* diskCache = new DiskCache<Ret,Args...>(key,pickle,unpickle,funcName);
  PersistentMemoized<T,Ret,Args...> memoized(diskCache);
  return memoized;
}

template<typename T, typename Ret, typename... Args>
PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&), string funcName){
  static_assert(std::is_base_of<PersistentMemoizable<Ret,Args...>, T>::value, 
    "Must Memoize a class that inherits from PersistentMemoizable");

  DiskCache<Ret,Args...>* diskCache = new DiskCache<Ret,Args...>(key,pickle,unpickle,funcName);
  PersistentMemoized<T,Ret,Args...> memoized(diskCache);
  return memoized;
}

// template<typename Ret>
// string ser(Ret x){
//   string ret = "";
//   ret << x;
//   return ret;
// }

// template<typename Ret>
// Ret deser(string x){
//   Ret ret;
//   x >> ret;
//   return ret;
// }

// template<typename... Args>
// string key(Args... args){
//   string ret = "";
//   ((ret << args), ...);
//   return ret;
// }

// template<typename T, typename Ret, typename... Args>
// PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(){
//   DiskCache<Ret,Args...>* diskCache = new DiskCache<Ret,Args...>(key<Args...>,ser<Ret>,deser<Ret>,"test");
//   PersistentMemoized<T,Ret,Args...> memoized(diskCache);
//   return memoized;
// }