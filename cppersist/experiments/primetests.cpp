#include "../local.hpp"
#include "../mongo.hpp"
#include "../utils/log.hpp"
#include "data-generation/generators.cpp"
#include "examples/primesAlt.cpp"
#include "examples/primesAlt2.cpp"
#include "../utils/files.hpp"
#include <string>
#include <set>
#include <iostream>
typedef unsigned long long largestUnsigned;
using namespace std::chrono;
using namespace cpst;
using namespace std;
#define PRIME_NUM_INPUT 300000

int smallestPrime(int n){
  for(int i=2;i<=n;i++){
    if(n%i==0) return i;
  }
  return 0;
} 

list<int> primeIterative(int n) {
  list<int> primeFactors;
  while (n > 1) {
    int nextPrime = smallestPrime(n);
    primeFactors.push_back(nextPrime);
    n /= nextPrime;
  }
  return primeFactors;
}

void runPrimesAlt2TestsSeq(){
  auto localMemo = getLocalMemoizedObj<PrimeFactorizerAlt2>(primesAlt2Key,primesAlt2Pickle,primesAlt2Unpickle,"primestest2",primeHash);
  list<int> input;
  for(int i=2;i<=(PRIME_NUM_INPUT+2);i++){
    input.push_back(i);
  }
  largestUnsigned totalTimeUnmemoized = 0;
  std::list<int>::iterator it;
  for (it = input.begin(); it != input.end(); it++)
  {
    auto start = high_resolution_clock::now();
    primeIterative(*it);
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
  appendRowToFile("./data/primesAlt2Seq.csv",row);
}

void runPrimesAlt2TestWRep(int seed){
  auto localMemo = getLocalMemoizedObj<PrimeFactorizerAlt2>(primesAlt2Key,primesAlt2Pickle,primesAlt2Unpickle,"primestest2",primeHash);
  IntGenerator ig(seed,2,PRIME_NUM_INPUT+2);
  list<int> input;
  for(int i=0;i<PRIME_NUM_INPUT;i++){
    int n = stoi(ig.getNext());
    input.push_back(n);
  }
  largestUnsigned totalTimeUnmemoized = 0;
  std::list<int>::iterator it;
  for (it = input.begin(); it != input.end(); it++)
  {
    auto start = high_resolution_clock::now();
    primeIterative(*it);
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
  appendRowToFile("./data/primesAlt2WRep.csv",row);
}

void runPrimesAlt2TestWORep(int seed){
  srand (seed); 
  vector<int> temp_input;
  list<int> input;
  for(int i=2;i<=(PRIME_NUM_INPUT+2);i++){
    temp_input.push_back(i);
  }
  while(!temp_input.empty()){
    int randIndex = rand()%temp_input.size();
    input.push_back(temp_input[randIndex]);
    temp_input.erase(temp_input.begin()+randIndex);
  }
  largestUnsigned totalTimeUnmemoized = 0;
  std::list<int>::iterator it;
  for (it = input.begin(); it != input.end(); it++)
  {
    auto start = high_resolution_clock::now();
    primeIterative(*it);
    auto timeTaken = duration_cast<nanoseconds>(high_resolution_clock::now()-start).count();
    totalTimeUnmemoized += timeTaken;
  }
  largestUnsigned totalTimeMemoized = 0;
  auto localMemo = getLocalMemoizedObj<PrimeFactorizerAlt2>(primesAlt2Key,primesAlt2Pickle,primesAlt2Unpickle,"primestest2",primeHash);
  for (it = input.begin(); it != input.end(); it++)
  {
    int n = *it;
    localMemo(n);
    totalTimeMemoized += localMemo.solveTime;
  }
  string row = to_string(totalTimeUnmemoized) + "," + to_string(totalTimeMemoized);
  appendRowToFile("./data/primesAlt2WORep.csv",row);
}