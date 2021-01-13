#ifndef PERSISTER_H_FILE
#define PERSISTER_H_FILE

#include "../interfaces/cache.hpp"
#include "../interfaces/memcache.hpp"
#include <string>
#include <mutex>
#include <future>
using std::string;
namespace cpst{
  template<typename T, typename Ret, typename ...Args>
  class PersistentMemoized: public T{
    public:
      PersistentMemoized(MemCache<Ret,Args...>* primaryCache, Cache<Ret,Args...>* secondaryCache);
      PersistentMemoized(Cache<Ret,Args...>* primaryCache);
      PersistentMemoized(const PersistentMemoized& lvalue);
      PersistentMemoized(PersistentMemoized&& rvalue);
      ~PersistentMemoized();
      Ret operator()(Args const&... args);
      PersistentMemoized& operator=(PersistentMemoized&& rvalue);
      PersistentMemoized& operator=(const PersistentMemoized& lvalue);
      void printCaches();
      void resetMetrics();
    private:
      long cacheHits;
      long cacheMisses;
      long timeTaken; //wall clock time in ms
      std::future<void> discard;
      void write(Args const&..., Ret const&);
      std::mutex cacheConsistent;
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

  enum MemCacheType { REGULAR, LRU_CACHE, ONE };

  template<typename Ret, typename... Args>
  MemCache<Ret,Args...>* getMemoryCache(MemCacheType type,string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&));
}
#include "persister.cpp"
#endif