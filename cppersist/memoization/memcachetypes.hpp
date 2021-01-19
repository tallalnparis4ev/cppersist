/**
 * @file
 * Defines the different memory caches that can be used
 */
enum MemCacheType { 
  /** A memory cache of unbounded size */
  REGULAR, 
  /** 
   * A memory cache of maximum size = 1000 entries, by default. 
   * The eviction policy is least recently used. The size of this can 
   * be changed by calling setMemoryCache(LRU_CACHE, desired size).
   */
  LRU_CACHE, 
  /** A memory cache of maximum size = one */
  ONE 
};