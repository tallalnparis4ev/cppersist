#include <iostream>
#include <iterator>
#include <list>

#include "../local.hpp"
using std::list;
using namespace std;
// Prime Factors Example
class PrimeFactorizer : public PersistentMemoizable<list<int>, int> {
 public:
  list<int> solve(int n) override {
    cout << n << endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
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

string keymaker(int n) { return std::to_string(n); }

int main() {
  PrimeFactorizer primeFactorizer;
  PersistentMemoized memoizedPrimes = getLocalMemoizedObj<PrimeFactorizer>(
      keymaker, listIntsToString, intListStrToList, "primesAlt");
  list<int> z = memoizedPrimes(25);
  std::cout << listIntsToString(z) << std::endl;
}