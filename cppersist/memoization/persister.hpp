#ifndef PERSISTER_H_FILE
#define PERSISTER_H_FILE

#include "../interfaces/cache.hpp"
#include "../interfaces/memcache.hpp"
#include <string>
#include <mutex>
#include <future>
using std::string;
namespace cpst{
  enum MemCacheType { REGULAR, LRU_CACHE, ONE };

  template<typename T, typename Ret, typename ...Args>
  class PersistentMemoized: public T{
    public:
      PersistentMemoized(MemCacheType primaryCache, Cache<Ret,Args...>* secondaryCache);
      PersistentMemoized(Cache<Ret,Args...>* primaryCache);
      PersistentMemoized(const PersistentMemoized& lvalue);
      PersistentMemoized(PersistentMemoized&& rvalue);
      ~PersistentMemoized();
      Ret operator()(Args const&... args);
      PersistentMemoized& operator=(PersistentMemoized&& rvalue);
      PersistentMemoized& operator=(const PersistentMemoized& lvalue);
      void resetMetrics();
      void setMemoryCache(MemCacheType);
      void setMemoryCache(MemCacheType, int);
      size_t solveTime = 0; //wall clock time in ms
      size_t cacheHits = 0;
      size_t cacheMisses = 0;
      size_t hitTime = 0; //wall clock time in ms
      size_t missTime = 0; //wall clock time in ms
      size_t missPenalty = 0; //wall clock time in ms
      bool miss = false;
      void resetTimes();
    private:
      void nullFields();
      void copy(const PersistentMemoized& lvalue);
      void move(PersistentMemoized&& rvalue); 
      MemCacheType memCacheType;
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
}
#include "persister.cpp"
#endif