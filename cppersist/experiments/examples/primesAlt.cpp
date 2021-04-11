#include <iostream>
#include <iterator>
#include <list>

#include "../../local.hpp"

using std::list;
using namespace std;
using namespace cpst;
// Prime Factors Example
class PrimeFactorizer : public Memoizable<list<int>, int> {
 public:
  list<int> solve(int n) override {
    list<int> primeFactors;
    while (n > 1) {
      int nextPrime = smallestPrime(n);
      primeFactors.push_back(nextPrime);
      n /= nextPrime;
    }
    return primeFactors;
  }
  int smallestPrime(int n) {  // memoize this instead
    for (int i = 2; i <= n; i++) {
      if (n % i == 0) return i;
    }
    return -1;
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

string primesAltKey(int n) { return std::to_string(n); }
string primeHash(string key) { return key; }