#include "../local.hpp"
#include "../mongo.hpp"
#include "../utils/log.hpp"
#include "data-generation/generators.cpp"
#include "examples/fib.cpp"
#include "examples/primesAlt2.cpp"
#include "../utils/files.hpp"
#include <string>
using namespace cpst;
using namespace std;
void runTests(const string& version){
  NumberGenerator nextNum(4);
  nextNum.max = 15;
  auto mongoPrime = getMongoMemoizedObj<PrimeFactorizer>
  (primeKey,listIntsToString,intListStrToList,"localhost:5000","primes");
  auto mongoFib = getMongoMemoizedObj<FibonacciSolver>
  (fibKey,intostr,strtoi,"localhost:5000","fib");
  int numberOfRuns = 1000;
  auto start = high_resolution_clock::now();
  for(int i=0;i<numberOfRuns;i++){
    int nextInput = strtoi(nextNum.getNext());
    mongoPrime(nextInput);
    mongoFib(nextInput);
  }
  auto end = high_resolution_clock::now();
  auto solveTime = duration_cast<nanoseconds>(end-start).count();
  appendRowToFile("./"+version+"Mongo"+".out",to_string(solveTime));
}

int main(int argc, char const *argv[])
{
  std::string mode = argv[1];
  runTests(mode);
  return 0;
}
