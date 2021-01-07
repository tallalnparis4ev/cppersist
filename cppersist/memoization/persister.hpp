#ifndef PERSISTER_H_FILE
#define PERSISTER_H_FILE

#include "../interfaces/cache.hpp"
#include "../interfaces/memcache.hpp"
#include <string>
using std::string;

template<typename T, typename Ret, typename ...Args>
class PersistentMemoized: public T{
  public:
    PersistentMemoized(MemCache<Ret,Args...>* primaryCache, Cache<Ret,Args...>* secondaryCache);
    PersistentMemoized(Cache<Ret,Args...>* primaryCache);
    PersistentMemoized(const PersistentMemoized& lvalue);
    PersistentMemoized(PersistentMemoized&& rvalue);
    ~PersistentMemoized();
    Ret operator()(Args... args);
    PersistentMemoized& operator=(PersistentMemoized&& rvalue);
    PersistentMemoized& operator=(const PersistentMemoized& lvalue);
    void printCaches();
  private:
    void deleteCaches();
    Cache<Ret,Args...>* primaryCache;
    Cache<Ret,Args...>* secondaryCache;
    virtual Ret solve(Args... args);
};

template<typename Ret, typename... Args>
class PersistentMemoizable
{
  public:
    virtual Ret solve(Args... args) = 0;
};

enum MemCacheType { REGULAR, LRU_CACHE };

template<typename Ret, typename... Args>
MemCache<Ret,Args...>* getMemoryCache(MemCacheType type,string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&));

#include "persister.cpp"
#endif