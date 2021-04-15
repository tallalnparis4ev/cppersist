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

TEST(Fibonacci, TestMongoCache) {//mongo
  auto fibSolver =
      getMongoMemoizedObj<FibonacciSolver>(keymaker, intostr, strtoi, "localhost:5000", "test");
  ASSERT_EQ(fibSolver.getCache()->get(8), nullopt); //test Cache object
  ASSERT_EQ(fibSolver(8), 13);
  ASSERT_EQ(fibSolver.getCache()->get(8).value_or(2), 13); //test Cache object
}

TEST(Fibonacci, FineInputFS) { //filesystem
  auto fibSolver =
      getLocalMemoizedObj<FibonacciSolver>(keymaker, intostr, strtoi, "fib");
  ASSERT_EQ(fibSolver(8), 13); //ensure correct answer
  removeDir(fibOutPath);
}

TEST(Fibonacci, FineInputMongo) { //mongo
  auto fibSolver =
      getMongoMemoizedObj<FibonacciSolver>(keymaker, intostr, strtoi, "localhost:5000", "test");
  ASSERT_EQ(fibSolver(8), 13); //ensure correct answer
}

TEST(Fibonacci, FineInputMem) { //in-memory
  auto fibSolver =
      getMemoizedObj<FibonacciSolver>(keymaker, intostr, strtoi);
  ASSERT_EQ(fibSolver(8), 13); //ensure correct answer
}

TEST(Fibonacci, TestFSCache) {//filesystem
  auto fibSolver =
      getLocalMemoizedObj<FibonacciSolver>(keymaker, intostr, strtoi, "fib");
  ASSERT_EQ(fibSolver.getCache()->get(8),nullopt); //test Cache object
  ASSERT_EQ(fibSolver(8), 13);
  ASSERT_EQ(fibSolver.getCache()->get(8).value_or(2), 13); //test Cache object
}

TEST(Fibonacci, TestMemCache) {//in-memory
  auto fibSolver =
      getMemoizedObj<FibonacciSolver>(keymaker, intostr, strtoi);
  ASSERT_EQ(fibSolver.getCache()->get(8),nullopt); //test Cache object
  ASSERT_EQ(fibSolver(8), 13);
  ASSERT_EQ(fibSolver.getCache()->get(8), 13); //test Cache object
}