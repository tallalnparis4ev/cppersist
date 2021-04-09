#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "../local.hpp"
#include "./helpers/Timer.cpp"
#include "../utils/files.hpp"
#include "../utils/log.hpp"
#include "data-generation/generators.cpp"
typedef unsigned long long largestUnsigned;
using namespace std::chrono;
using namespace cpst;
using namespace std;

class PrimeSolver {
 public:
  virtual list<int> solve(int n) = 0;
};

class PrimeRec : public Memoizable<list<int>, int>,
                 public PrimeSolver {
 public:
  list<int> solve(int n) override {
    int nextPrime = smallestPrime(n);
    if (n / nextPrime == 1) {
      list<int> primeFactors;
      primeFactors.push_front(nextPrime);
      return primeFactors;
    } else {
      list<int> primeFactors = solve(n / nextPrime);
      primeFactors.push_front(nextPrime);
      return primeFactors;
    }
  }

  int smallestPrime(int n) {
    if (n % 2 == 0) return 2;
    int root = sqrt(n);
    for (int i = 3; i <= root; i += 2) {
      if (n % i == 0) return i;
    }
    return n;
  }
};

// class PrimeIter : public Memoizable<list<int>, int>,
//                   public PrimeSolver {
//  public:
//   list<int> solve(int n) override {
//     list<int> primes;
//     // Print the number of 2s that divide n
//     while (n % 2 == 0) {
//       primes.push_back(2);
//       n = n / 2;
//     }
//     int root = sqrt(n);
//     for (int i = 3; i <= root; i = i + 2) {
//       while (n % i == 0) {
//         primes.push_back(i);
//         n = n / i;
//       }
//     }
//     if (n > 2) primes.push_back(n);

//     return primes;
//   }
// };

list<int> primesUnpickle(string primeString) {
  list<int> primeFactors;
  string curPrime = "";
  for (char& cur : primeString) {
    if (cur != ' ') {
      curPrime += cur;
    } else {
      primeFactors.push_back(std::stoi(curPrime));
      curPrime = "";
    }
  }
  return primeFactors;
}
string primesPickle(list<int> primes) {
  string ret = "";
  for (int prime : primes) {
    ret += (std::to_string(prime) + " ");
  }
  return ret;
}

string primesKey(int n) { return std::to_string(n); }

void runPrime(PrimeSolver& solver, vector<int>& input, string outPath,
              bool cppersist) {
  Timer timer;
  timer.start();
  for (vector<int>::iterator it = input.begin(); it != input.end(); it++) {
    list<int> answer = solver.solve(*it);
  }
  timer.end();
  appendRowToFile(outPath, timer.getRow());
}

void runPrime(vector<int>& input, string type, bool cppersist, bool recursive,
              bool keepCache) {
  string outPath = getOutPath("Primes", type, cppersist, recursive, keepCache);
  if (recursive) {
    PrimeRec rec;
    auto localMemo = getLocalMemoizedObj<PrimeRec>(
        primesKey, primesPickle, primesUnpickle, "primesTest", identity<string>);
    if (!cppersist) {
      runPrime(rec, input, outPath, cppersist);
    } else {
      runPrime(localMemo, input, outPath, cppersist);
    }
  } 
  else {
    PrimeIter iter;
    auto localMemo = getLocalMemoizedObj<PrimeIter>(
        primesKey, primesPickle, primesUnpickle, "primesTest", identity<string>);
    if (!cppersist) {
      runPrime(iter, input, outPath, cppersist);
    } else {
      runPrime(localMemo, input, outPath, cppersist);
    }
  }
}

void runPrimeSeq(vector<int>& input, bool cppersist, bool recursive,
                 bool keepCache) {
  runPrime(input, "Seq", cppersist, recursive, keepCache);
}

void runPrimeWORep(vector<int>& input, bool cppersist, bool recursive,
                   bool keepCache, int seed) {
  shuffle(input.begin(), input.end(), default_random_engine(seed));
  runPrime(input, "WORep", cppersist, recursive, keepCache);
}

void runPrimeWRep(vector<int>& input, bool cppersist, bool recursive,
                  bool keepCache, int seed) {
  srand(seed);
  vector<int> newInp;
  while (newInp.size() != input.size()) {
    int toAdd = input[rand() % input.size()];
    newInp.push_back(toAdd);
  }
  runPrime(newInp, "WRep", cppersist, recursive, keepCache);
}

vector<int> generateInput(int n) {
  vector<int> ret;
  for (int i = 2; i <= n; i++) {
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
    runPrimeSeq(input, cppersist, recursive, keepCache);
  }

  if (std::strcmp(version, "worep") == 0) {
    runPrimeWORep(input, cppersist, recursive, keepCache, seed);
  }

  if (std::strcmp(version, "wrep") == 0) {
    runPrimeWRep(input, cppersist, recursive, keepCache, seed);
  }
  return 0;
}
