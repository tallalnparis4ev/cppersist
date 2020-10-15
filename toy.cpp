#include "persister.hpp"
#include <iostream>
//Fibonacci Sequence Example
class FibonacciSolver: public PersistentMemoizable<int, int>{
  public:
    int solve(int n) override {
      std::cout << "Original" << std::endl;
      if(n==0) return 0;
      if(n==1) return 1;
      return solve(n-1) + solve(n-2);
    } 
};
int strtoi(string lol){
  return std::stoi(lol);
}
string intostr(int lol){
  return std::to_string(lol);
}

string keymaker(int lol){
  return std::to_string(lol);
}
int main(){
  FibonacciSolver fibonacciSolver;
  PersistentMemoized memoizedFib = Persister::getMemoizedObj<FibonacciSolver>(fibonacciSolver,keymaker,intostr,strtoi);
  int z = memoizedFib(3);
  std::cout << z << std::endl;
}

