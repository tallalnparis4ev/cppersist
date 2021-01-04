#include "../local.hpp"
#include <iostream>
#include <list> 
#include <iterator> 
using std::list;
using namespace std;
// Prime Factors Example
class PrimeFactorizer: public PersistentMemoizable<list<int>, int>{
  public:
    list<int> solve(int n) override {
      list <int> primeFactors; 
      std::cout << "Original " << n << std::endl;
      while(n>1){
        int nextPrime = smallestPrime(n);
        primeFactors.push_back(nextPrime);
        n /= nextPrime;
      }
      return primeFactors;
    }
    int smallestPrime(int n){
      for(int i=2;i<=n;i++){
        if(n%i==0) return i;
      }
      return -1;
    } 
};

list<int> intListStrToList(string primeString){
  list <int> primeFactors; 
  string curPrime = "";
  for(char& cur : primeString) {
    if(cur != ' '){
      curPrime += cur;
    }
    else{
      primeFactors.push_back(std::stoi(curPrime));
      curPrime = "";
    }
  }
  return primeFactors;
}
string listIntsToString(list<int> primes){
  string ret = "";
  for(int prime : primes){
    ret += (std::to_string(prime) + " ");
  }
  return ret;
}

string keymaker(int n){
  return std::to_string(n);
}

int main(){
  PersistentMemoized memoizedPrimes = getLocalMemoizedObj<PrimeFactorizer>(keymaker,listIntsToString,intListStrToList,"primesAlt");
  list<int> z = memoizedPrimes(1000);
  std::cout << listIntsToString(z) << std::endl;
}