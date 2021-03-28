#include "cppersist/local.hpp"
#include "cppersist/mongo.hpp"
#include "gtest/gtest.h"
using namespace cpst;
class FibonacciSolver : public Memoizable<int, int> {
 public:
  int solve(int n) override {
    std::cout << "Original" << std::endl;
    if (n == 1) return 0;
    if (n == 2) return 1;
    return solve(n - 1) + solve(n - 2);
  }
};

int strtoi(string x) { return std::stoi(x); }
string intostr(int x) { return std::to_string(x); }

string keymaker(int x) { return std::to_string(x); }

TEST(Fibonacci, FineInput) {
  auto fibSolver =
      getLocalMemoizedObj<FibonacciSolver>(keymaker, intostr, strtoi, "fib");
  ASSERT_EQ(fibSolver(8), 13);
}