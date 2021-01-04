#include "../mongo.hpp"
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

int strtoi(const string& x){
  return std::stoi(x);
}
string intostr(const int& x){
  return std::to_string(x);
}

string keymaker(const int& x){
  return std::to_string(x);
}
using namespace std;
int main(){
  FibonacciSolver fibonacciSolver;
  // PersistentMemoized memoizedFib = getLocalMemoizedObj<FibonacciSolver>(keymaker,intostr,strtoi);
  // PersistentMemoized memoizedFib2 = getLocalMemoizedObj<FibonacciSolver>(keymaker,intostr,strtoi,"fib");
  PersistentMemoized memoizedFibDb1 = getMongoMemoizedObj<FibonacciSolver>(keymaker,intostr,strtoi,
  "http://localhost:5000");
  // int z = memoizedFib(3);
  // int z = memoizedFib2(3);
  int z = memoizedFibDb1(3);
  std::cout << z << std::endl;

}