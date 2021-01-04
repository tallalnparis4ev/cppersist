#ifndef MONGO_H_FILE
#define MONGO_H_FILE

#include "persister.hpp"

template<typename T, typename Ret, typename... Args>
PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string dbURL);

template<typename T, typename Ret, typename... Args>
PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string dbURL, string funcName);

#include "mongo.cpp"
#endif