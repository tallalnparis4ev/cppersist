#ifndef LOCAL_H_FILE
#define LOCAL_H_FILE

#include "memoization/persister.hpp"

template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&));

template<typename T, typename Ret, typename... Args>
PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&));

template<typename T, typename Ret, typename... Args>
PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(MemCacheType primaryCache,string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&));

#include "local.cpp"
#endif