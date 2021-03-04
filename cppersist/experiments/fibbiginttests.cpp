#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "../local.hpp"
#include "../utils/files.hpp"
#include "../utils/log.hpp"
#include "data-generation/generators.cpp"
#include "helpers/bigint.cpp"
typedef unsigned long long largestUnsigned;
using namespace std::chrono;
using namespace cpst;
using namespace std;

class FibSolver {
 public:
  virtual bigint solve(int n) = 0;
};

class FibIter : public PersistentMemoizable<bigint, int>, public FibSolver {
 public:
  bigint solve(int n) override {
    if (n == 0) return 0;
    if (n == 1) return 1;
    bigint prev = 0;
    bigint cur = 1;
    while (n >= 2) {
      bigint sum = prev + cur;
      prev = cur;
      cur = sum;
      n--;
    }
    return cur;
  };
};

class FibRec : public PersistentMemoizable<bigint, int>, public FibSolver {
 public:
  bigint solve(int n) override {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return solve(n - 1) + solve(n - 2);
  };
};

bigint fibBIUnpickle(string x) { return bigint(x); }

string fibBIPickle(bigint x) { return x.to_string(); }

string fibBIKey(int x) { return std::to_string(x); }

string fibBIHash(string key) { return key; }

void runFib(FibSolver& solver, vector<int>& input, string path,
            bool cppersist) {
  largestUnsigned time = 0;
  for (vector<int>::iterator it = input.begin(); it != input.end(); it++) {
    auto start = high_resolution_clock::now();
    bigint answer = solver.solve(*it);
    auto timeTaken =
        duration_cast<nanoseconds>(high_resolution_clock::now() - start)
            .count();
    time += timeTaken;
  }
  appendRowToFile(path, to_string(time));
}

void runFib(vector<int>& input, string type, bool cppersist, bool recursive,
            bool keepCache) {
  string path = getOutPath("Fib", type, cppersist, recursive, keepCache);
  if (recursive) {
    FibRec rec;
    auto localMemo = getLocalMemoizedObj<FibRec>(
        fibBIKey, fibBIPickle, fibBIUnpickle, "fibTest", fibBIHash);
    if (!cppersist) {
      runFib(rec, input, path, cppersist);
    } else {
      runFib(localMemo, input, path, cppersist);
    }
  } else {
    FibIter iter;
    auto localMemo = getLocalMemoizedObj<FibIter>(
        fibBIKey, fibBIPickle, fibBIUnpickle, "fibTest", fibBIHash);
    if (!cppersist) {
      runFib(iter, input, path, cppersist);
    } else {
      runFib(localMemo, input, path, cppersist);
    }
  }
}

void runFibTestSeq(vector<int>& input, bool cppersist, bool recursive,
                   bool keepCache) {
  runFib(input, "Seq", cppersist, recursive, keepCache);
}

void runFibTestWORep(vector<int>& input, bool cppersist, bool recursive,
                     bool keepCache, int seed) {
  shuffle(input.begin(), input.end(), default_random_engine(seed));
  runFib(input, "WORep", cppersist, recursive, keepCache);
}

void runFibTestWRep(vector<int>& input, bool cppersist, bool recursive,
                    bool keepCache, int seed) {
  srand(seed);
  vector<int> newInp;
  while (newInp.size() != input.size()) {
    int toAdd = input[rand() % input.size()];
    newInp.push_back(toAdd);
  }
  runFib(newInp, "WRep", cppersist, recursive, keepCache);
}

vector<int> generateInput(int n) {
  vector<int> ret;
  for (int i = 0; i <= n; i++) {
    ret.push_back(i);
  }
  return ret;
}

int main(int argc, char const* argv[]) {
  int numInput = stoi(argv[1]);
  vector<int> input = generateInput(numInput);
  bool cppersist = stoi(argv[2]);
  bool recursive = stoi(argv[3]);
  bool keepCache = stoi(argv[4]);
  int seed = stoi(argv[5]);
  const char* version = argv[6];

  if (std::strcmp(version, "seq") == 0) {
    runFibTestSeq(input, cppersist, recursive, keepCache);
  }

  if (std::strcmp(version, "worep") == 0) {
    runFibTestWORep(input, cppersist, recursive, keepCache, seed);
  }

  if (std::strcmp(version, "wrep") == 0) {
    runFibTestWRep(input, cppersist, recursive, keepCache, seed);
  }

  return 0;
}
