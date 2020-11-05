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
    //()
    //solveMemoized()
};
int strtoi(string x){
  return std::stoi(x);
}
string intostr(int x){
  return std::to_string(x);
}

string keymaker(int x){
  return std::to_string(x);
}
int main(){
  FibonacciSolver fibonacciSolver;
  // PersistentMemoized memoizedFib = Persister::getLocalMemoizedObj<FibonacciSolver>(fibonacciSolver,keymaker,intostr,strtoi);
  PersistentMemoized memoizedFib = Persister::getMongoMemoizedObj<FibonacciSolver>(fibonacciSolver,keymaker,intostr,strtoi);
  PersistentMemoized memoizedFib2 = Persister::getMongoMemoizedObj<FibonacciSolver>(fibonacciSolver,keymaker,intostr,strtoi);
  int z = memoizedFib(3);
  int y = memoizedFib2(3);
  std::cout << z << std::endl;
  std::cout << y << std::endl;
}