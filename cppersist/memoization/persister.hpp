#ifndef PERSISTER_H_FILE
#define PERSISTER_H_FILE

#include "../interfaces/cache.hpp"
#include "../interfaces/memcache.hpp"
#include "./memcachetypes.hpp"
#include <string>
#include <mutex>
#include <future>
using std::string;
namespace cpst{
  /**
   * An object of this class will have its function call operator () to perform
   * some computation, and this function has persistent memoization applied.
   */
  template<typename T, typename Ret, typename ...Args>
  class PersistentMemoized: public T{
    public:
      PersistentMemoized(MemCacheType primaryCache, Cache<Ret,Args...>* secondaryCache);
      PersistentMemoized(Cache<Ret,Args...>* primaryCache);
      /**
       * Copy Constructor
       */
      PersistentMemoized(const PersistentMemoized& lvalue);
      /**
       * Move Constructor
       */
      PersistentMemoized(PersistentMemoized&& rvalue);
      /**
       * Destructor
       */
      ~PersistentMemoized();
      Ret operator()(Args const&... args);
      /**
       * Move Assignment
       */
      PersistentMemoized& operator=(PersistentMemoized&& rvalue);
      /**
       * Copy Assignment
       */
      PersistentMemoized& operator=(const PersistentMemoized& lvalue);
      /**
       * A method to change/assign a memory cache
       */
      void setMemoryCache(MemCacheType);
      /**
       * A method to change/assign a memory cache, followed by the size of the
       * cache. The size is only applicable if the MemCacheType is LRU_CACHE.
       * The size will determine the maximum number of entries in the LRU_CACHE.
       */
      void setMemoryCache(MemCacheType, int);
    private:
      void nullFields();
      void copy(const PersistentMemoized& lvalue);
      void move(PersistentMemoized&& rvalue); 
      /** Indicates the current memory cache being used */
      MemCacheType memCacheType;
      std::future<void> discard;
      void write(Args const&..., Ret const&);
      std::mutex cacheConsistent;
      void deleteCaches();
      /**
       * This is the first cache that is searched. If a memory cache is set
       * then this will always correspond to a memory cache.
       */
      Cache<Ret,Args...>* primaryCache;
      /**
       * This cache will be searched if the primaryCache is missed. This cache
       * can only be a persistent cache.
       */
      Cache<Ret,Args...>* secondaryCache;
      virtual Ret solve(Args... args);
  };

  /**
   * This is an abstract class, and a class which a user wants to memoize
   * must inherit from this class.
   */
  template<typename Ret, typename... Args>
  class PersistentMemoizable
  {
    public:
      /**
       * This is a user-defined solve function, which the user wants to
       * memoize. It can be recursive (i.e. call solve()), and these
       * recursive calls will also be memoized. For the memoization to be
       * accurate, this function must be pure. 
       */
      virtual Ret solve(Args... args) = 0;
  };
}
#include "persister.cpp"
#endif