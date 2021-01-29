#include "../local.hpp"
#include <vector>
#include "../utils/log.hpp"
#include "./examples/primesAlt2.cpp"
using namespace cpst;



template<typename T,typename Ret, typename Args>
void runTests(ArgGenerator argGenerator, PersistentMemoizable<T,Ret,Args...> memoized){
  while (argGenerator.hasNext())
  {
    long totalTimeMs = 0;
    
  }
}
int main(int argc, char const *argv[])
{
  PrimeFactorizer test;
  string x = listIntsToString(test.solve(2147483647));
  log(x);
  // PrimeGenerator prime(2);
  log("hello");
  /* code */
  return 0;
}
