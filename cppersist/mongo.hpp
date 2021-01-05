#ifndef MONGO_H_FILE
#define MONGO_H_FILE

#include "memoization/persister.hpp"

template<typename T, typename Ret, typename... Args>
PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&), string dbURL);

template<typename T, typename Ret, typename... Args>
PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&), string dbURL, string funcName);

template<typename T, typename Ret, typename... Args>
PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(MemCacheType type,string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&), string dbURL);

template<typename T, typename Ret, typename... Args>
PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(MemCacheType type,string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&), string dbURL, string funcName);

#include "mongo.cpp"
#endif