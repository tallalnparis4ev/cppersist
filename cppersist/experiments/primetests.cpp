#include "../local.hpp"
#include "../utils/log.hpp"
#include "data-generation/generators.cpp"
#include "../utils/files.hpp"
#include <string>
#include <set>
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
typedef unsigned long long largestUnsigned;
using namespace std::chrono;
using namespace cpst;
using namespace std;

class PrimeSolver{
  public: 
    virtual list<int> solve(int n) = 0;
};

class PrimeRec: public PersistentMemoizable<list<int>, int>, public PrimeSolver{
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
      if (n % 2 == 0) 
          return 2; 
      for (int i = 3; i * i <= n; i += 2) { 
          if (n % i == 0) 
              return i; 
      } 
      return n; 
    }
  };

class PrimeIter: public PersistentMemoizable<list<int>, int>, public PrimeSolver{
  public:
    list<int> solve(int n) override
    {  
      list<int> primes;
      // Print the number of 2s that divide n  
      while (n % 2 == 0)  
      {  
        primes.push_back(2);
        n = n/2;  
      }  
    
      for (int i = 3; i <= sqrt(n); i = i + 2)  
      {  
          while (n % i == 0)  
          {  
            primes.push_back(i);
            n = n/i;  
          }  
      }  
      if (n > 2)  
          primes.push_back(n);

      return primes;
    }  
  };

list<int> primesUnpickle(string primeString) {
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
string primesPickle(list<int> primes) {
  string ret = "";
  for (int prime : primes) {
    ret += (std::to_string(prime) + " ");
  }
  return ret;
}

string primesKey(int n){
  return std::to_string(n);
}


void runPrime(PrimeSolver& solver, vector<int>& input, string path, bool cppersist){
  largestUnsigned time = 0;
  for (vector<int>::iterator it = input.begin(); it != input.end(); it++){
    // auto start = high_resolution_clock::now();
    list<int> answer = solver.solve(*it);
    // auto timeTaken = duration_cast<nanoseconds>(high_resolution_clock::now()-start).count();
    // time += timeTaken;
  }
  // appendRowToFile(path,to_string(time));
}


void runPrime(vector<int>& input, string type, bool cppersist, bool recursive, bool keepCache){
  // string path = getOutPath("Primes",type,cppersist,recursive,keepCache);
  string path = "";
  if(recursive){
    PrimeRec rec;
    auto localMemo = 
      getLocalMemoizedObj<PrimeRec>(primesKey,primesPickle,primesUnpickle,"primesTest",noHash);
    if(!cppersist){
      runPrime(rec,input,path,cppersist);
    }
    else{
      runPrime(localMemo,input,path,cppersist);
    }
  }
  else{
    PrimeIter iter;
    auto localMemo = 
      getLocalMemoizedObj<PrimeIter>(primesKey,primesPickle,primesUnpickle,"primesTest",noHash);
    if(!cppersist){
      runPrime(iter,input,path,cppersist);
    }
    else{
      runPrime(localMemo,input,path,cppersist);
    }
  }
}

void runPrimeSeq(vector<int>& input, bool cppersist, bool recursive, bool keepCache){
  runPrime(input,"Seq",cppersist,recursive,keepCache);
}

void runPrimeWORep(vector<int>& input, bool cppersist, bool recursive, bool keepCache, int seed){
  shuffle(input.begin(), input.end(), default_random_engine(seed));
  runPrime(input,"WORep",cppersist,recursive,keepCache);
}

void runPrimeWRep(vector<int>& input, bool cppersist, bool recursive, bool keepCache, int seed){
  srand(seed);
  vector<int> newInp;
  while(newInp.size() != input.size()){
    int toAdd = input[rand()%input.size()];
    newInp.push_back(toAdd);
  }
  runPrime(newInp,"WRep",cppersist,recursive,keepCache);
}

vector<int> generateInput(int n){
  vector<int> ret;
  for(int i=2;i<=n;i++){
    ret.push_back(i);
  }
  // for(int x : ret){
    // cout << x << endl;
  // }
  return ret;
}

int main(int argc, char const *argv[])
{
  int numInput = stoi(argv[1]);
  vector<int> input = generateInput(numInput);
  bool cppersist = stoi(argv[2]);
  bool recursive = stoi(argv[3]);
  bool keepCache = stoi(argv[4]);
  int seed = stoi(argv[5]);
  const char* version = argv[6];

  if (std::strcmp(version, "seq") == 0)
  {
    runPrimeSeq(input,cppersist,recursive,keepCache);
  }

  if (std::strcmp(version, "worep") == 0)
  {
    runPrimeWORep(input,cppersist,recursive,keepCache,seed);
  }

  if (std::strcmp(version, "wrep") == 0)
  {
    runPrimeWRep(input,cppersist,recursive,keepCache,seed);
  }

  return 0;
}
