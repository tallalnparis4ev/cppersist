#include "cppersist/local.hpp"
#include "cppersist/mongo.hpp"
#include "cppersist/memory.hpp"
#include "gtest/gtest.h"
#include "utils/files.hpp"
using namespace cpst;
class FibonacciSolver : public Memoizable<int, int> {
 public:
  int solve(int n) override {
    if (n == 1) return 0;
    if (n == 2) return 1;
    return solve(n - 1) + solve(n - 2);
  }
};

int strtoi(string x) { return std::stoi(x); }
string intostr(int x) { return std::to_string(x); }

string keymaker(int x) { return std::to_string(x); }

string fibOutPath = "./persist/fib";

TEST(Fibonacci, FineInputFS) {
  auto fibSolver =
      getLocalMemoizedObj<FibonacciSolver>(keymaker, intostr, strtoi, "fib");
  ASSERT_EQ(fibSolver(8), 13);
  removeDir(fibOutPath);
}

TEST(Fibonacci, FineInputMongo) {
  auto fibSolver =
      getMongoMemoizedObj<FibonacciSolver>(keymaker, intostr, strtoi, "fib");
  ASSERT_EQ(fibSolver(8), 13);
}

TEST(Fibonacci, FineInputMem) {
  auto fibSolver =
      getMemoizedObj<FibonacciSolver>(keymaker, intostr, strtoi);
  ASSERT_EQ(fibSolver(8), 13);
}

TEST(Fibonacci, TestMongoCache) {
  auto fibSolver =
      getMongoMemoizedObj<FibonacciSolver>(keymaker, intostr, strtoi, "localhost:5000");
  ASSERT_EQ(fibSolver(8), 13);
  ASSERT_EQ(fibSolver.getCache()->get(8).value_or(2), 13);
}

TEST(Fibonacci, TestFSCache) {
  auto fibSolver =
      getLocalMemoizedObj<FibonacciSolver>(keymaker, intostr, strtoi, "fib");
  ASSERT_EQ(fibSolver.getCache()->get(8),nullopt);
  ASSERT_EQ(fibSolver(8), 13);
  ASSERT_EQ(fibSolver.getCache()->get(8).value_or(2), 13);
}

TEST(Fibonacci, TestMemCache) {
  auto fibSolver =
      getMemoizedObj<FibonacciSolver>(keymaker, intostr, strtoi);
  ASSERT_EQ(fibSolver.getCache()->get(8),nullopt);
  ASSERT_EQ(fibSolver(8), 13);
  ASSERT_EQ(fibSolver.getCache()->get(8), 13);
}