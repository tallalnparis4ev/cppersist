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
  /**
   * A method to change/assign a memory cache
   */
  void setMemoryCache(MemCacheType);
  /**
   * A method to change/assign a memory cache, followed by the size of the
   * cache. The size is only applicable if the MemCacheType is LRU or REGULAR.
   * The size will determine the maximum number of entries in the LRU or REGULAR.
   */
  void setMemoryCache(MemCacheType, int);

  /**
   * A method that returns a boolean indicating if this set of input arguments
   * should be memoized or not.
   */
  void setDecision(bool (*decision)(Args...));

  /**
   * A method to change the location of your cache.
   * If a filesystem cache this will save entries in the directory 'loc/funcName'.
   * If a MongoDB cache this will save entries in to the database at the URL 'loc'.
   */
  void setCacheLoc(string loc);

  /**
   * Returns the underlying Cache object, if a hybrid cache is used this will be
   * a MemCache object. Otherwise it will be a PerCache object.
   */ 
  Cache<Ret, Args...>* getCache(){
    return this->primaryCache;
  }

  /**
   * Returns the underlying Cache object, this can only be a PerCache object. 
   * If a hybrid cache is not set, this function doesn't have a defined return.
   */ 
  Cache<Ret, Args...>* getSecondaryCache(){
    return this->secondaryCache;
  }

  virtual Ret solve(Args... args);

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
