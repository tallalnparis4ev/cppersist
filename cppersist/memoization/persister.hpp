#ifndef PERSISTER_H_FILE
#define PERSISTER_H_FILE

#include "../interfaces/cache.hpp"
#include "../interfaces/memcache.hpp"

template<typename T, typename Ret, typename ...Args>
class PersistentMemoized: public T{
  public:
    PersistentMemoized(MemCache<Ret,Args...>* primaryCache, Cache<Ret,Args...>* secondaryCache);
    PersistentMemoized(Cache<Ret,Args...>* primaryCache);
    ~PersistentMemoized();
    Ret operator()(Args... args);
  private:
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

#include "persister.cpp"
#endif