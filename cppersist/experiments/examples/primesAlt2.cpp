#include "../../local.hpp"
#include <iostream>
#include <iterator>
#include <list>

#include "../local.hpp"
using std::list;
using namespace std;
using namespace cpst;
// Prime Factors Example
class PrimeFactorizerAlt2: public PersistentMemoizable<list<int>, int>{
  public:
    list<int> solve(int n) override {
      int nextPrime = smallestPrime(n);
      if(n/nextPrime == 1){
        list<int> primeFactors;
        primeFactors.push_front(nextPrime);
        return primeFactors;
      }
      else{
        list<int> primeFactors = solve(n/nextPrime);
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

string primesAlt2Key(int n){
  return std::to_string(n);
}

// int main(){
//   PrimeFactorizer primeFactorizer;
//   PersistentMemoized memoizedPrimes = getLocalMemoizedObj<PrimeFactorizer>(keymaker,listIntsToString,intListStrToList,"primesAlt");
//   list<int> z = memoizedPrimes(25);
//   std::cout << listIntsToString(z) << std::endl;
// }
