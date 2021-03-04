#include <algorithm>
#include <chrono>
#include <iostream>
// #include <unordered_map>
#include <algorithm>
#include <functional>
#include <map>
#include <random>
#include <vector>

#include "../local.hpp"
#include "../utils/files.hpp"
#include "helpers/bigint.cpp"
typedef unsigned long long largestUnsigned;
using namespace std::chrono;
using namespace std;
using namespace cpst;

class NKSolver {
 public:
  virtual bigint solve(int n, int k) = 0;
};

bigint binom(int n, int k) {
  bigint value = 1;
  for (int i = 0; i < k; i++) value = (value * (n - i)) / (i + 1);
  return value;
}

class NKIter : public PersistentMemoizable<bigint, int, int>, public NKSolver {
 public:
  bigint solve(int n, int k) override { return 2; };
};

class NKRec : public PersistentMemoizable<bigint, int, int>, public NKSolver {
 public:
  bigint solve(int n, int k) override {
    if (n == k || k == 0) return 1;
    return solve(n - 1, k - 1) + solve(n - 1, k);
  }
};

string binKey(int n, int k) { return to_string(n) + "," + to_string(k); }

string binPickle(bigint result) { return result.to_string(); }

bigint binUnpickle(string result) { return bigint(result); }

string binHash(string key) { return key; }

void runBin(NKSolver& solver, vector<pair<int, int>>& input, string path,
            bool cppersist) {
  largestUnsigned time = 0;
  for (vector<pair<int, int>>::iterator it = input.begin(); it != input.end();
       it++) {
    auto start = high_resolution_clock::now();
    bigint answer = solver.solve(it->first, it->second);
    auto timeTaken =
        duration_cast<nanoseconds>(high_resolution_clock::now() - start)
            .count();
    time += timeTaken;
  }
  appendRowToFile(path, to_string(time));
}

void runBin(vector<pair<int, int>>& input, string type, bool cppersist,
            bool recursive, bool keepCache) {
  string path = getOutPath("Bin", type, cppersist, recursive, keepCache);
  if (recursive) {
    NKRec rec;
    auto localMemo = getLocalMemoizedObj<NKRec>(binKey, binPickle, binUnpickle,
                                                "binTest", binHash);
    if (!cppersist) {
      runBin(rec, input, path, cppersist);
    } else {
      runBin(localMemo, input, path, cppersist);
    }
  } else {
    NKIter iter;
    auto localMemo = getLocalMemoizedObj<NKIter>(binKey, binPickle, binUnpickle,
                                                 "binTest", binHash);
    if (!cppersist) {
      runBin(iter, input, path, cppersist);
    } else {
      runBin(localMemo, input, path, cppersist);
    }
  }
}

void runFibTestSeq(vector<pair<int, int>>& input, bool cppersist,
                   bool recursive, bool keepCache) {
  runBin(input, "Seq", cppersist, recursive, keepCache);
}

void runFibTestWORep(vector<pair<int, int>>& input, bool cppersist,
                     bool recursive, bool keepCache, int seed) {
  shuffle(input.begin(), input.end(), default_random_engine(seed));
  runBin(input, "WORep", cppersist, recursive, keepCache);
}

void runFibTestWRep(vector<pair<int, int>>& input, bool cppersist,
                    bool recursive, bool keepCache, int seed) {
  srand(seed);
  vector<pair<int, int>> newInp;
  while (newInp.size() != input.size()) {
    pair<int, int> toAdd = input[rand() % input.size()];
    newInp.push_back(toAdd);
  }
  runBin(newInp, "WRep", cppersist, recursive, keepCache);
}

vector<pair<int, int>> generatePairs(int n) {
  vector<pair<int, int>> ret;
  for (int i = 0; i <= n; i++) {
    for (int j = 0; j <= i; j++) {
      ret.push_back(pair(i, j));
    }
  }
  return ret;
}

int main(int argc, char const* argv[]) {
  cout << binom(10000, 5000) << endl;
  int numInput = stoi(argv[1]);
  vector<pair<int, int>> NKs = generatePairs(numInput);
  bool cppersist = stoi(argv[2]);
  bool recursive = stoi(argv[3]);
  bool keepCache = stoi(argv[4]);
  int seed = stoi(argv[5]);
  const char* version = argv[6];

  if (std::strcmp(version, "seq") == 0) {
    runBinSeq(input, cppersist, recursive, keepCache);
  }

  if (std::strcmp(version, "worep") == 0) {
    runBinWORep(input, cppersist, recursive, keepCache, seed);
  }

  if (std::strcmp(version, "wrep") == 0) {
    runBinWRep(input, cppersist, recursive, keepCache, seed);
  }

  return 0;
}
