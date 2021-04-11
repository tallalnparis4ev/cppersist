#include <iostream>
#include <iterator>
#include <list>

#include "../../local.hpp"
#include "../local.hpp"
using std::list;
using namespace std;
// Prime Factors Example
class PrimeFactorizer : public Memoizable<list<int>, int> {
 public:
  list<int> solve(int n) override {
    list<int> primeFactors;
    std::cout << "Original " << n << std::endl;
    int curPrime = 2;
    while (n >= (curPrime * curPrime)) {
      if (n % curPrime == 0) {
        primeFactors.push_back(curPrime);
        n = n / curPrime;
      } else
        curPrime++;
    }
    primeFactors.push_back(n);
    return primeFactors;
  }
};

list<int> intListStrToList(string primeString) {
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
string listIntsToString(list<int> primes) {
  string ret = "";
  for (int prime : primes) {
    ret += (std::to_string(prime) + " ");
  }
  return ret;
}

string keymaker(int n) { return std::to_string(n); }

int main() {
  PersistentMemoized memoizedPrimes = getLocalMemoizedObj<PrimeFactorizer>(
      keymaker, listIntsToString, intListStrToList, "primes");
  list<int> z = memoizedPrimes(20);
  std::cout << listIntsToString(z) << std::endl;
}