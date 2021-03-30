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
                              &intSimple::unpickle, &sha256, "localhost:5000");
  test.put(1, 1);
  EXPECT_EQ(test.get(1).value_or(2), 1)
      << "===================================================" << std::endl
      << "The reason for this failure is most likely because" << std::endl
      << "the MongoDB server is not running on localhost:5000" << std::endl
      << "===================================================" << std::endl;
  ASSERT_EQ(test.get(2), nullopt);
}