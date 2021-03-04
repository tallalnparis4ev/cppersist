#include <iostream>
#include <iterator>
#include <list>

#include "../../local.hpp"
#include "../local.hpp"
using std::list;
using namespace std;
using namespace cpst;
// Prime Factors Example
class PrimeFactorizerAlt2 : public PersistentMemoizable<list<int>, int> {
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
    for (int i = 3; i * i <= n; i += 2) {
      if (n % i == 0) return i;
    }
    return n;
  }
};

list<int> primesAlt2Unpickle(string primeString) {
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
string primesAlt2Pickle(list<int> primes) {
  string ret = "";
  for (int prime : primes) {
    ret += (std::to_string(prime) + " ");
  }
  return ret;
}

string primesAlt2Key(int n) { return std::to_string(n); }
