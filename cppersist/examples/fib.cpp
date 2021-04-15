#include <iostream>

#include "../local.hpp"
#include "../memory.hpp"
#include "../mongo.hpp"
using namespace cpst;
// Fibonacci Sequence Example
class FibonacciSolver : public Memoizable<int, int> {
 public:
  int solve(int n) override {
    std::cout << "Original" << std::endl;
    if (n == 0) return 0;
    if (n == 1) return 1;
    return solve(n - 1) + solve(n - 2);
  }
};

int strtoi(string x) { return std::stoi(x); }
string intostr(int x) { return std::to_string(x); }
string keymaker(int x) { return std::to_string(x); }
bool decision(int x){
  return true;
}

using namespace std;
int main() {
  FibonacciSolver fibonacciSolver;
  Memoized memoized2 =
      getMemoizedObj<FibonacciSolver>(keymaker, intostr, strtoi);
  memoized2.getCache();
  PersistentMemoized memoized =
      getLocalMemoizedObj<FibonacciSolver>(keymaker, intostr, strtoi, "fib");
  memoized.setDecision(decision);
  memoized.getCache();
  memoized.setCacheLoc("../apples");
  // Memoized memoized = getMemoizedObj<FibonacciSolver>(
      // keymaker, intostr, strtoi, MemCacheType::LRU, 10, decision);
  // Memoized memoized = getMemoizedObj<FibonacciSolver>(
      // keymaker, intostr, strtoi);
  // memoized.setDecision(decision);
  // PersistentMemoized memoizedFibDb1 = getMongoMemoizedObj<FibonacciSolver>(
  // keymaker, intostr, strtoi, "http://localhost:5000");
  int x = memoized(3);
  x = memoized(3);
  // int y = memoizedFibDb1(3);
  // int z = memoizedFib(3);
}
