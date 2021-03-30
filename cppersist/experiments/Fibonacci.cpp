#include <iostream>
#include <set>
#include <string>

#include "../local.hpp"
#include "../utils/files.hpp"
#include "../utils/log.hpp"
#include "helpers/Timer.cpp"
#include "data-generation/generators.cpp"
#include <random>
typedef unsigned long long largestUnsigned;
using namespace std::chrono;
using namespace cpst;
using namespace std;
#define FIB_NUM_INPUT 100000
// can only calculate upto, and including, 93rd (0-index) fib number
#define MAX_FIB_INPUT 93

class FibSolver {
 public:
  virtual largestUnsigned solve(int) = 0;
};

class FibRec : public Memoizable<largestUnsigned, int>,
                 public FibSolver {
 public:
  largestUnsigned solve(int n) override {
    if(n==0) return 0;
    if(n==1) return 1;
    return solve(n-1) + solve(n-2);
  }
};

class FibIter : public Memoizable<largestUnsigned, int>,
                 public FibSolver {
 public:
  largestUnsigned solve(int n) override {
    if (n == 0) return 0;
    if (n == 1) return 1;
    largestUnsigned prev = 0;
    largestUnsigned cur = 1;
    while (n >= 2) {
      largestUnsigned sum = prev + cur;
      prev = cur;
      cur = sum;
      n--;
    }
    return cur;
  }
};

string fibKey(int x) { return std::to_string(x); }
largestUnsigned fibUnpickle(string x) { return std::stoull(x); }
string fibPickle(largestUnsigned x) { return std::to_string(x); }

void runFib(FibSolver& solver, vector<int>& input, string path) {
  Timer timer;
  timer.start();
  for (vector<int>::iterator it = input.begin(); it != input.end(); it++) {
    largestUnsigned answer = solver.solve(*it);
  }
  timer.end();
  appendRowToFile(path, timer.getRow());
}


void runFib(vector<int>& input, string type,
              bool cppersist, bool recursive, bool keepCache) {
  string path = getOutPath("Fibonacci", type, cppersist, recursive, keepCache);
  if (recursive) {
    FibRec rec;
    auto localMemo = getLocalMemoizedObj<FibRec>(
        fibKey, fibPickle, fibUnpickle, "fibTest", identity<string>);
    if (!cppersist) {
      runFib(rec, input, path);
    } else {
      runFib(localMemo, input, path);
    }
  } else {
    FibIter iter;
    auto localMemo = getLocalMemoizedObj<FibIter>(
        fibKey, fibPickle, fibUnpickle, "fibTest", identity<string>);
    if (!cppersist) {
      runFib(iter, input, path);
    } else {
      runFib(localMemo, input, path);
    }
  }
}

void runFibWORep(vector<int>& input, bool cppersist,
                   bool recursive, bool keepCache, int seed) {
  shuffle(input.begin(), input.end(), default_random_engine(seed));
  runFib(input,"WORep", cppersist, recursive, keepCache);
}

void runFibWRep(vector<int>& input,  bool cppersist,
                  bool recursive, bool keepCache, int seed) {
  srand(seed);
  vector<int> newInp;
  while (newInp.size() != input.size()) {
    newInp.push_back(input[rand() % input.size()]);
  }
  runFib(newInp,"WRep", cppersist, recursive, keepCache);
}


int main(int argc, char const* argv[]) {

  int numInput = stoi(argv[1]);
  vector<int> input;
  for(int i=0;i<numInput;i++){
    input.push_back(i);
  }
  bool cppersist = stoi(argv[2]);
  bool recursive = stoi(argv[3]);
  bool keepCache = stoi(argv[4]);
  int seed = stoi(argv[5]);
  const char* version = argv[6];

  if (std::strcmp(version, "worep") == 0) {
    runFibWORep(input, cppersist, recursive, keepCache, seed);
  }

  if (std::strcmp(version, "wrep") == 0) {
    runFibWRep(input, cppersist, recursive, keepCache, seed);
  }

  return 0;
}