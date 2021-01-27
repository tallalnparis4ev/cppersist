#include "gtest/gtest.h"
#include "tests/fib.cpp"
#include "tests/memcache.cpp"
#include "tests/percache.cpp"

int main(int argc, char** argv) {
  /* code */
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
