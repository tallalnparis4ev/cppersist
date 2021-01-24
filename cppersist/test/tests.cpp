// #include "googletest/include/gtest/gtest.h"
#include "cppersist/local.hpp"
#include "gtest/gtest.h"
#include "tests/fib.cpp"
using namespace std;
TEST(HELLO, WORLD) { ASSERT_EQ(2, 2); }

int main(int argc, char** argv) {
  /* code */
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
