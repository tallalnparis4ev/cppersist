#include "cppersist/cacheimpl/memory/lrucache.hpp"
#include "cppersist/cacheimpl/memory/onecache.hpp"
#include "cppersist/cacheimpl/memory/regcache.hpp"
#include "gtest/gtest.h"
#include "utils/intSimple.hpp"
using namespace cpst;

TEST(LRUCacheTests, WorkingCorrectly) {
  LRUCache<int, int> cache(3, &intSimple::pickle, &intSimple::pickle,
                          &intSimple::unpickle);
  cache.put(1, 1);
  cache.put(2, 1);
  ASSERT_EQ(cache.get(2).value(), 1);
  cache.put(3, 1);  // max size
  cache.put(4, 1);  // the entry (1,1) should be removed because it is the least
                   // recently used
  ASSERT_EQ(cache.get(1), nullopt);
}

TEST(OneCacheTests, WorkingCorrectly) {
  OneCache<int, int> cache(&intSimple::pickle, &intSimple::pickle,
                          &intSimple::unpickle);
  cache.put(1, 1);
  ASSERT_EQ(cache.get(1).value(), 1);
  cache.put(2, 1);
  ASSERT_EQ(cache.get(2).value(), 1);
  ASSERT_EQ(cache.get(1), nullopt);
}

TEST(RegCacheTests, WorkingCorrectly) {
  RegCache<int, int> cache(3, &intSimple::pickle, &intSimple::pickle,
                          &intSimple::unpickle);
  cache.put(1, 1);
  cache.put(2, 1);
  ASSERT_EQ(cache.get(2).value(), 1);
  ASSERT_EQ(cache.get(1).value(), 1);
}
