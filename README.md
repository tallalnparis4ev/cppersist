# C++ Persistent Memoization : cppersist

## TLDR

cppersist is a small library which allows users' to apply persistent memoization to a member function. cppersist allows member functions to be memoized via disk storage or mongoDB.

Here is an example of how cppersist is used:

```c++
#include <cppersist/disk.h> //if you need to memoize with the local filesystem
#include <cppersist/mongo.h> //if you need to memoize with MongoDB
#include <iostream>

//The class to be memoized - must implement a solve method
class FibonacciSolver: public PersistentMemoizable<int, int>{
  public:
    int solve(int n) override {
      if(n<=1) return n;
      return fib(n-1) + fib(n-2);
    }
};

//Pickle/serialize function
std::string intostr(int x){
  return std::to_string(x);
}
//Unpickle/deserialize function
int strtoi(std::string x){
  return std::stoi(x);
}
//Key function
std::string keymaker(int x){
  return std::to_string(x);
}

int main(int argc, char** argv) {
  PersistentMemoized memoizedFib = getLocalMemoizedObj<DFSSolver>(keymaker,intostr,strtoi); //disk cache
  std::cout << memoizedFib(2) << std::endl;
  memoizedFib = getMongoMemoizedObj<DFSSolver>(keymaker,intostr,strtoi); //mongo cache
  std::cout << memoizedFib(2) << std::endl;
}
```

## Documentation

[![Documentation](https://img.shields.io/badge/docs-online-informational?style=for-the-badge&link=https://tallalnparis4ev.github.io/)](https://tallalnparis4ev.github.io/)  
This is the documentation of cppersist: [here](https://tallalnparis4ev.github.io/). 

## Usage
To build your project, utilising cppersist, via CMake (version 3.0+ required):
```cmake
include(FetchContent) 
FetchContent_Declare(cppersist GIT_REPOSITORY https://github.com/tallalnparis4ev/cppersist GIT_TAG master) 
FetchContent_MakeAvailable(cppersist)
include_directories(${cppersist_SOURCE_DIR}) 
#Remove the following 4 lines if the MongoDB cache is not used 
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG c8d33915dbd88ad6c92b258869b03aba06587ff9) 
FetchContent_MakeAvailable(cpr) 
find_package(OpenSSL REQUIRED) #Remove this if you're communicating with the MongoDB server via HTTP and HTTPS is not needed
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE cpr::cpr) 
```
