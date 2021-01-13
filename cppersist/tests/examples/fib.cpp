#include "../../mongo.hpp"
#include "../../local.hpp"
#include <iostream>
//Fibonacci Sequence Example
class FibonacciSolver: public PersistentMemoizable<int, int>{
  public:
    int solve(int n) override {
      // std::cout << "Original" << std::endl;
      if(n==0) return 0;
      if(n==1) return 1;
      return solve(n-1) + solve(n-2);
    }
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

#include <iostream>       // std::cout, std::endl
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using namespace std;
#include <list>         // std::chrono::seconds

template<typename T> 
list<T*> makeGenericList(T* infer){
  list<T*> x;
  return x;
}

int main(){
  // PersistentMemoized memoizedFib = getLocalMemoizedObj<FibonacciSolver>(keymaker,intostr,strtoi);
  // PersistentMemoized memoizedFib = getLocalMemoizedObj<FibonacciSolver>(REGULAR,keymaker,intostr,strtoi);
  // PersistentMemoized memoizedFib = getLocalMemoizedObj<FibonacciSolver>(REGULAR,keymaker,intostr,strtoi,"fib");
  // PersistentMemoized memoizedFib = getLocalMemoizedObj<FibonacciSolver>(keymaker,intostr,strtoi,"fib");

  // PersistentMemoized memoizedFib = getMongoMemoizedObj<FibonacciSolver>(LRU_CACHE,keymaker,intostr,strtoi,"localhost:5000");
  // memoizedFib(2);
  // memoizedFib.printCaches();
  // PersistentMemoized memoizedFib2 = std::move(memoizedFib);
  // PersistentMemoized memoizedFib3 = memoizedFib2;
  // PersistentMemoized memoizedFib4 = getMongoMemoizedObj<FibonacciSolver>(LRU_CACHE,keymaker,intostr,strtoi,"localhost:5000");
  // memoizedFib4 = memoizedFib2;
  // memoizedFib4 = getMongoMemoizedObj<FibonacciSolver>(LRU_CACHE,keymaker,intostr,strtoi,"localhost:5000");
  // cout << memoizedFib2(2) << endl;
  // cout << memoizedFib4(2) << endl;

  // PersistentMemoized memoizedFib = getLocalMemoizedObj<FibonacciSolver>(LRU_CACHE,keymaker,intostr,strtoi);
  // memoizedFib(2);
  // memoizedFib.printCaches();
  // PersistentMemoized memoizedFib2 = std::move(memoizedFib);
  // PersistentMemoized memoizedFib3 = memoizedFib2;
  // PersistentMemoized memoizedFib4 = getLocalMemoizedObj<FibonacciSolver>(LRU_CACHE,keymaker,intostr,strtoi);
  // memoizedFib4 = memoizedFib2;
  // memoizedFib4 = getLocalMemoizedObj<FibonacciSolver>(LRU_CACHE,keymaker,intostr,strtoi);
  // cout << memoizedFib2(2) << endl;
  // cout << memoizedFib4(2) << endl;


  PersistentMemoized memoizedFib = getMongoMemoizedObj<FibonacciSolver>(LRU_CACHE,keymaker,intostr,strtoi,"localhost:5000");
  std::this_thread::sleep_for (std::chrono::seconds(1));
  memoizedFib = getMongoMemoizedObj<FibonacciSolver>(REGULAR,keymaker,intostr,strtoi,"localhost:5000","test1");
  cout << memoizedFib(2) << endl;
  
    std::this_thread::sleep_for (std::chrono::seconds(1));

   memoizedFib = getMongoMemoizedObj<FibonacciSolver>(keymaker,intostr,strtoi,"localhost:5000","test2");
  cout << memoizedFib(2) << endl;
  
    std::this_thread::sleep_for (std::chrono::seconds(1));

   memoizedFib = getMongoMemoizedObj<FibonacciSolver>(keymaker,intostr,strtoi,"localhost:5000");
  cout << memoizedFib(2) << endl;
  
    std::this_thread::sleep_for (std::chrono::seconds(1));

  memoizedFib = getLocalMemoizedObj<FibonacciSolver>(REGULAR,keymaker,intostr,strtoi);
  cout << memoizedFib(2) << endl;
  
    std::this_thread::sleep_for (std::chrono::seconds(1));

  memoizedFib = getLocalMemoizedObj<FibonacciSolver>(LRU_CACHE,keymaker,intostr,strtoi,"test1");
  cout << memoizedFib(2) << endl;
  
    std::this_thread::sleep_for (std::chrono::seconds(1));

  memoizedFib = getLocalMemoizedObj<FibonacciSolver>(keymaker,intostr,strtoi);
  cout << memoizedFib(2) << endl;
  
    std::this_thread::sleep_for (std::chrono::seconds(1));

  memoizedFib = getLocalMemoizedObj<FibonacciSolver>(keymaker,intostr,strtoi,"test2");
  cout << memoizedFib(2) << endl;
  
  // PersistentMemoized memoizedFib = getLocalMemoizedObj<FibonacciSolver>(keymaker,intostr,strtoi);
  // int z = memoizedFib(3);
  // std::cout << z << std::endl;

}

// template<typename T>
// void swap(T& a, T& b)
// {
//   T temp(std::move (a));
//   a = std::move (b);
//   b = std::move (t);
// }