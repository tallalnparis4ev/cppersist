#ifndef LOCAL_H_FILE
#define LOCAL_H_FILE

#include "persister.hpp"

template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string));

template<typename T, typename Ret, typename... Args>
PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string funcName);

#include "local.cpp"
#endif