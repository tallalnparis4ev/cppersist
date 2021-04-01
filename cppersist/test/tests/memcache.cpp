#include "cppersist/cacheimpl/memory/lrucache.hpp"
#include "cppersist/cacheimpl/memory/onecache.hpp"
#include "cppersist/cacheimpl/memory/regcache.hpp"
#include "utils/intSimple.hpp"

TEST(LRUCacheTests, WorkingCorrectly) {
  LRUCache<int, int> test(3, &intSimple::pickle, &intSimple::pickle,
                          &intSimple::unpickle);
  test.put(1, 1);
  test.put(2, 1);
  ASSERT_EQ(test.get(2).value(), 1);
  test.put(3, 1);  // max size
  test.put(4, 1);  // the entry (1,1) should be removed because it is the least
                   // recently used
  ASSERT_EQ(test.get(1), nullopt);
}

TEST(OneCacheTests, WorkingCorrectly) {
  OneCache<int, int> test(&intSimple::pickle, &intSimple::pickle,
                          &intSimple::unpickle);
  test.put(1, 1);
  ASSERT_EQ(test.get(1).value(), 1);
  test.put(2, 1);
  ASSERT_EQ(test.get(2).value(), 1);
  ASSERT_EQ(test.get(1), nullopt);
}

TEST(RegCacheTests, WorkingCorrectly) {
  RegCache<int, int> test(3, &intSimple::pickle, &intSimple::pickle,
                          &intSimple::unpickle);
  test.put(1, 1);
  test.put(2, 1);
  ASSERT_EQ(test.get(2).value(), 1);
  ASSERT_EQ(test.get(1).value(), 1);
}
