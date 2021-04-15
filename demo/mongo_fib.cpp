#include <cppersist/mongo.hpp> //if you need to memoize with MongoDB
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
  cpst::PersistentMemoized mongoMemoizedFib = cpst::getMongoMemoizedObj<FibonacciSolver>(keymaker,serialize,deserialize,"localhost:5000"); //mongo cache
  std::cout << mongoMemoizedFib(3) << std::endl;
}