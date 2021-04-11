#ifndef PERSISTER_H_FILE
#define PERSISTER_H_FILE

#include <future>
#include <mutex>
#include <string>

#include "../../interfaces/cache.hpp"
#include "../../interfaces/memcache.hpp"
#include "../memcachetypes.hpp"
#include "../memoizable.hpp"
using std::string;
namespace cpst {
typedef unsigned long long largestUnsigned;
/**
 * An object of this class will have its function call operator () to perform
 * some computation, and this function has persistent memoization applied.
 */
template <typename T, typename Ret, typename... Args>
class PersistentMemoized : public T {
 public:
  PersistentMemoized(MemCacheType primaryCache,
                     Cache<Ret, Args...>* secondaryCache);
  PersistentMemoized(Cache<Ret, Args...>* primaryCache);
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
  void resetMetrics();
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
  largestUnsigned solveTime = 0;  // wall clock time in ns
  largestUnsigned cacheHits = 0;
  largestUnsigned cacheMisses = 0;
  largestUnsigned hitTime = 0;      // wall clock time in ms
  largestUnsigned missTime = 0;     // wall clock time in ms
  largestUnsigned missPenalty = 0;  // wall clock time in ms
  bool miss = false;
  void resetTimes();
  virtual Ret solve(Args... args);
  void setDecision(bool (*decision)(Args...));

 private:
  void nullFields();
  void copy(const PersistentMemoized& lvalue);
  void move(PersistentMemoized&& rvalue);
  std::future<void> discard;
  void write(Args const&..., Ret const&);
  std::mutex cacheConsistent;
  void deleteCaches();
  /**
   * This is the first cache that is searched. If a memory cache is set
   * then this will always correspond to a memory cache.
   */
  Cache<Ret, Args...>* primaryCache;
  /**
   * This cache will be searched if the primaryCache is missed. This cache
   * can only be a persistent cache.
   */
  Cache<Ret, Args...>* secondaryCache;
  bool (*decision)(Args...) = returnTrue<Args...>;
};
}  // namespace cpst
#include "memoizer.cpp"
#endif