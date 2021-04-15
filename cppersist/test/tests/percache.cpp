#include "cppersist/cacheimpl/persistent/diskcache.hpp"
#include "cppersist/cacheimpl/persistent/mongodbcache.hpp"
#include "cppersist/crypto/sha256.h"
#include "utils/intSimple.hpp"

TEST(DiskCacheTests, WorkingCorrectly) {
  DiskCache<int, int> test(&intSimple::pickle, &intSimple::pickle,
                           &intSimple::unpickle, &sha256, "test");
  test.put(1, 1);
  ASSERT_EQ(test.get(1).value(), 1);
  ASSERT_EQ(test.get(2), nullopt);
}

TEST(MongoDBCacheTests, WorkingCorrectly) {
  MongoDBCache<int, int> test(&intSimple::pickle, &intSimple::pickle,
                              &intSimple::unpickle, &sha256, "localhost:5000",
                              "test");
  test.put(100, 1);  // put (100,1) in cache
  EXPECT_EQ(test.get(100).value_or(2),
            1)  // make sure key 100 returns the value 1
      << "===================================================" << std::endl
      << "The reason for this failure is most likely because" << std::endl
      << "the MongoDB server is not running on localhost:5000" << std::endl
      << "===================================================" << std::endl;
  ASSERT_EQ(test.get(101), nullopt);  // make sure key 101 returns the value
                                      // nullopt - it's not in the cache
}