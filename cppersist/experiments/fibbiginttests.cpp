#include "../local.hpp"
#include "../mongo.hpp"
#include "../utils/log.hpp"
#include "data-generation/generators.cpp"
#include "examples/fib.cpp"
#include "../utils/files.hpp"
#include <string>
#include <set>
#include <iostream>
#include "helpers/bigint.cpp"
typedef unsigned long long largestUnsigned;
using namespace std::chrono;
using namespace cpst;
using namespace std;
#define FIB_BIG_INT_NUM_INPUT 5000 
#define MAX_FIB_BIG_INT_INPUT 5000


class FibBigIntSolver : public PersistentMemoizable<bigint, int>{
  public:
    bigint solve(int n) override{
      if(n==0) return 0;
      if(n==1) return 1;
      return solve(n-1) + solve(n-2);
    };
    string to_string(){
      stringstream ss;
      ss << *this;
      string s;
      ss >> s;
      return s;
	  };
};

bigint fibBIUnpickle(string x){
  return bigint(x);
}

string fibBIPickle(bigint x){
  return x.to_string();
}

string fibBIKey(int x){
  return std::to_string(x);
}

string fibBIHash(string key){
  return key;
}


bigint fibBigInt(int n){
  if(n==0) return 0;
  if(n==1) return 1;
  bigint prev = 0;
  bigint cur = 1;
  while(n>=2){
    bigint sum = prev + cur;
    prev = cur;
    cur = sum;
    n--;
  }
  return cur;
}


void runFibBITestSeq(){
  auto localMemo = getLocalMemoizedObj<FibBigIntSolver>(fibBIKey,fibBIPickle,fibBIUnpickle,"fibbi",fibBIHash);
  list<int> input;
  for(int i=0;i<FIB_BIG_INT_NUM_INPUT;i++){
    int next = i%(MAX_FIB_BIG_INT_INPUT+1);
    input.push_back(next);
  }
  largestUnsigned totalTimeUnmemoized = 0;
  std::list<int>::iterator it;
  for (it = input.begin(); it != input.end(); it++)
  {
    auto start = high_resolution_clock::now();
    fibBigInt(*it);
    auto timeTaken = duration_cast<nanoseconds>(high_resolution_clock::now()-start).count();
    totalTimeUnmemoized += timeTaken;
  }
  largestUnsigned totalTimeMemoized = 0;
  for (it = input.begin(); it != input.end(); it++)
  {
    int n = *it;
    localMemo(n);
    totalTimeMemoized += localMemo.solveTime;
  }
  string row = to_string(totalTimeUnmemoized) + ", " + to_string(totalTimeMemoized);
  appendRowToFile("./data/fibBISeq.csv",row);
}

void runFibBITestWRep(int seed){
  IntGenerator ng(seed,0,MAX_FIB_BIG_INT_INPUT);
  int numberInput = FIB_BIG_INT_NUM_INPUT;
  list<int> input;
  for(int i=0;i<numberInput;i++){
    int next = stoi(ng.getNext());
    if(next>ng.max || next < ng.min){
      cout << next << endl;
      return;
    }
    input.push_back(next);
  }
  largestUnsigned totalTimeUnmemoized = 0;
  std::list<int>::iterator it;
  for (it = input.begin(); it != input.end(); it++)
  {
    auto start = high_resolution_clock::now();
    fibBigInt(*it);
    auto timeTaken = duration_cast<nanoseconds>(high_resolution_clock::now()-start).count();
    totalTimeUnmemoized += timeTaken;
  }
  largestUnsigned totalTimeMemoized = 0;
  auto localMemo = getLocalMemoizedObj<FibBigIntSolver>(fibBIKey,fibBIPickle,fibBIUnpickle,"fibbi",fibBIHash);
  for (it = input.begin(); it != input.end(); it++)
  {
    int n = *it;
    localMemo(n);
    totalTimeMemoized += localMemo.solveTime;
  }
  string row = to_string(totalTimeUnmemoized) + ", " + to_string(totalTimeMemoized);
  appendRowToFile("./data/fibBIWRep",row);
}

void runFibBITestWORep(int seed){
  srand (seed); 
  vector<int> temp_input;
  list<int> input;
  while(input.size() < FIB_BIG_INT_NUM_INPUT){
    if(temp_input.size() == 0){
      for(int i=0;i<=MAX_FIB_BIG_INT_INPUT;i++){
        temp_input.push_back(i);
      }
    }
    int randIndex = rand()%temp_input.size();
    input.push_back(temp_input[randIndex]);
    temp_input.erase(temp_input.begin()+randIndex);
  }
  
  largestUnsigned totalTimeUnmemoized = 0;
  std::list<int>::iterator it;
  for (it = input.begin(); it != input.end(); it++)
  {
    auto start = high_resolution_clock::now();
    fibBigInt(*it);
    auto timeTaken = duration_cast<nanoseconds>(high_resolution_clock::now()-start).count();
    totalTimeUnmemoized += timeTaken;
  }
  largestUnsigned totalTimeMemoized = 0;
  auto localMemo = getLocalMemoizedObj<FibBigIntSolver>(fibBIKey,fibBIPickle,fibBIUnpickle,"fibbi",fibBIHash);
  for (it = input.begin(); it != input.end(); it++)
  {
    int n = *it;
    localMemo(n);
    totalTimeMemoized += localMemo.solveTime;
  }
  string row = to_string(totalTimeUnmemoized) + ", " + to_string(totalTimeMemoized);
  appendRowToFile("./data/fibBIWORep",row);
}