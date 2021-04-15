#include <cppersist/local.hpp> //if you need to memoize with your filesystem
// #include <cppersist/mongo.hpp> //if you need to memoize with MongoDB
#include <cppersist/memory.hpp> //if you need to memoize in-memory
#include <iostream>

//The class to be memoized - must implement a solve method
class FibonacciSolver: public cpst::Memoizable<int, int>{
  public:
    int solve(int n) override {
      if(n<=1) return n;
      return solve(n-1) + solve(n-2);
    }
};

//Pickle/serialize function
std::string serialize(int x){
  return std::to_string(x);
}
//Unpickle/deserialize function
int deserialize(std::string x){
  return std::stoi(x);
}
//Key function
std::string keymaker(int x){
  return std::to_string(x);
}

int main(int argc, char** argv) {
  cpst::PersistentMemoized memoizedFib = cpst::getLocalMemoizedObj<FibonacciSolver>(keymaker,serialize,deserialize); //disk cache
  std::cout << memoizedFib(3) << std::endl;

  cpst::Memoized memoizedInMemoryFib = cpst::getMemoizedObj<FibonacciSolver>(keymaker,serialize,deserialize); //in-memory cache
  std::cout << memoizedInMemoryFib(3) << std::endl;

  // cpst::PersistentMemoized mongoMemoizedFib = cpst::getMongoMemoizedObj<FibonacciSolver>(keymaker,serialize,deserialize,"localhost:5000"); //mongo cache
  // std::cout << mongoMemoizedFib(3) << std::endl;
}