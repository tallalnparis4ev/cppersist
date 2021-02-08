#include "../local.hpp"
#include "../mongo.hpp"
#include "../utils/log.hpp"
#include "data-generation/generators.cpp"
#include "examples/fib.cpp"
#include "../utils/files.hpp"
#include <string>
#include <set>
#include <iostream>
typedef unsigned long long largestUnsigned;
using namespace std::chrono;
using namespace cpst;
using namespace std;
#define FIB_NUM_INPUT 100000
//can only calculate upto, and including, 93rd (0-index) fib number
#define MAX_FIB_INPUT 93

largestUnsigned fibIterative(largestUnsigned n){
  if(n==0) return 0;
  if(n==1) return 1;
  largestUnsigned prev = 0;
  largestUnsigned cur = 1;
  while(n>=2){
    largestUnsigned sum = prev + cur;
    prev = cur;
    cur = sum;
    n--;
  }
  return cur;
}

void runFibTestWRep(int seed){
  IntGenerator ng(seed,0,93);
  int numberInput = FIB_NUM_INPUT;
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
    fibIterative(*it);
    auto timeTaken = duration_cast<nanoseconds>(high_resolution_clock::now()-start).count();
    totalTimeUnmemoized += timeTaken;
  }
  largestUnsigned totalTimeMemoized = 0;
  auto localMemo = getLocalMemoizedObj<FibonacciSolver>(fibKey,fibPickle,fibUnpickle,"fibtest",fibHash);
  for (it = input.begin(); it != input.end(); it++)
  {
    int n = *it;
    localMemo(n);
    totalTimeMemoized += localMemo.solveTime;
  }
  string row = to_string(totalTimeUnmemoized) + ", " + to_string(totalTimeMemoized);
  appendRowToFile("./data/fibIterativeWRep",row);
}

void runFibTestSeq(){
  auto localMemo = getLocalMemoizedObj<FibonacciSolver>(fibKey,fibPickle,fibUnpickle,"fibtest",fibHash);
  int numberInput = 100000;
  list<int> input;
  for(int i=0;i<numberInput;i++){
    int next = i%(MAX_FIB_INPUT+1);
    input.push_back(next);
  }
  largestUnsigned totalTimeUnmemoized = 0;
  std::list<int>::iterator it;
  for (it = input.begin(); it != input.end(); it++)
  {
    auto start = high_resolution_clock::now();
    fibIterative(*it);
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
  appendRowToFile("./data/fibIterativeSeq.csv",row);
}

void runFibTestWORep(int seed){
  srand (seed); 
  vector<int> temp_input;
  list<int> input;
  while(input.size() < FIB_NUM_INPUT){
    if(temp_input.size() == 0){
      for(int i=0;i<=MAX_FIB_INPUT;i++){
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
    fibIterative(*it);
    auto timeTaken = duration_cast<nanoseconds>(high_resolution_clock::now()-start).count();
    totalTimeUnmemoized += timeTaken;
  }
  largestUnsigned totalTimeMemoized = 0;
  auto localMemo = getLocalMemoizedObj<FibonacciSolver>(fibKey,fibPickle,fibUnpickle,"fibtest",fibHash);
  for (it = input.begin(); it != input.end(); it++)
  {
    int n = *it;
    localMemo(n);
    totalTimeMemoized += localMemo.solveTime;
  }
  string row = to_string(totalTimeUnmemoized) + ", " + to_string(totalTimeMemoized);
  appendRowToFile("./data/fibIterativeWORep",row);
}