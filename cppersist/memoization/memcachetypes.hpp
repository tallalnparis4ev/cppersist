#if !defined(MEM_CACHE_TYPES)
#define MEM_CACHE_TYPES
namespace cpst {
/**
 * @file
 * Defines the different memory caches that can be used
 */
enum class MemCacheType {
  /** A memory cache of unbounded size */
  REGULAR,
  /**
   * A memory cache of maximum size = 1000 entries, by default.
   * The eviction policy is least recently used. The size of this can
   * be changed by calling setMemoryCache(LRU, desired size).
   */
  LRU,
  /** A memory cache of maximum size = one */
  ONE
};
}  // namespace cpst
#endif  // MEM_CACHE_TYPES