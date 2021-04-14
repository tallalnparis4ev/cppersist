#include "gtest/gtest.h"
#include "cppersist/local.hpp"
#include "cppersist/mongo.hpp"
#include "cppersist/memory.hpp"
using namespace cpst;
#include "tests/fib.cpp"
#include "tests/memcache.cpp"
#include "tests/percache.cpp"
#include "tests/permemoizer.cpp"

int main(int argc, char** argv) {
  /* code */
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
