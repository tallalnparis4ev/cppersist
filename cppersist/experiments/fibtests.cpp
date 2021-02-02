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
#define NUM_INPUT 100000

//can only calculate upto 93rd (0-index) fib number
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
  int numberInput = NUM_INPUT;
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
  string row = to_string(totalTimeUnmemoized) + "," + to_string(totalTimeMemoized);
  appendRowToFile("./data/fibIterative",row);
}

void runFibTestSeq(){
  auto localMemo = getLocalMemoizedObj<FibonacciSolver>(fibKey,fibPickle,fibUnpickle,"fibtest",fibHash);
  int numberInput = 100000;
  list<int> input;
  for(int i=0;i<numberInput;i++){
    int next = i%94;
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
  string row = to_string(totalTimeUnmemoized) + "," + to_string(totalTimeMemoized);
  appendRowToFile("./data/fibIterativeSeq.csv",row);
}