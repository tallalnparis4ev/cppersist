#include <math.h>

#include <iostream>

#include "../local.hpp"
#include "../mongo.hpp"
using namespace cpst;
// Fibonacci Sequence Example
class Power : public PersistentMemoizable<int, int, int> {
 public:
  int solve(int x, int y) override {
    logOne("Original");
    return pow(x, y);
  }
};

std::string key(int x, int y) {
  return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
}
int strtoi(std::string x) { return std::stoi(x); }
std::string intostr(int x) { return std::to_string(x); }

using namespace std;
int main() {
  auto memoizedPow = getLocalMemoizedObj<Power>(key, intostr, strtoi, "power");
  int z = memoizedPow(2, 4);
  logOne(z);
  z = memoizedPow(10, 5);
  logOne(z);
}
