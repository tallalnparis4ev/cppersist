# C++ Persistent Memoization : cppersist

## TLDR

cppersist is a small library which allows users' to apply persistent memoization to a member function. cppersist allows member functions to be memoized via disk storage or mongoDB.

Here is an example of how cppersist is used:

```c++
#include <cppersist/local.hpp> //if you need to memoize with the local filesystem
#include <cppersist/mongo.hpp> //if you need to memoize with MongoDB
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
  cpst::PersistentMemoized memoizedFib = cpst::getLocalMemoizedObj<FibonacciSolver>(keymaker,intostr,strtoi); //disk cache
  std::cout << memoizedFib(2) << std::endl;
  memoizedFib = cpst::getMongoMemoizedObj<FibonacciSolver>(keymaker,intostr,strtoi,"localhost:5000"); //mongo cache
  std::cout << memoizedFib(2) << std::endl;
  cpst::Memoized memoizedInMemoryFib = cpst::getMemoizedObj<FibonacciSolver>(keymaker,intostr,strtoi); //in-memory cache
  std::cout << memoizedInMemoryFib(2) << std::endl;
}
```

## Documentation

[![Documentation](https://img.shields.io/badge/docs-online-informational?style=for-the-badge&link=https://tallalnparis4ev.github.io/)](https://tallalnparis4ev.github.io/)  
The documentation of cppersist is found [here](https://tallalnparis4ev.github.io/). Navigate to the 'modules' tab for documentation on the user-visible parts of cppersist.

## Usage
To build your project, utilising cppersist, via CMake (version 3.15.2+ required):
```cmake
cmake_minimum_required(VERSION 3.15.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#========= Change these lines for your project
project(demo)
add_executable(${CMAKE_PROJECT_NAME} fib.cpp)
#=============================================

#Get cppersist 
include(FetchContent)
FetchContent_Declare(cppersist GIT_REPOSITORY https://github.com/tallalnparis4ev/cppersist GIT_TAG master)
FetchContent_MakeAvailable(cppersist)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${cppersist_SOURCE_DIR}/cppersist/cmake)

#Link threading and filesystem libraries
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Filesystem REQUIRED)

include_directories(${cppersist_SOURCE_DIR})

#Get cpr if needed
if(HTTPS OR HTTP)
  FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/tallalnparis4ev/cpr.git GIT_TAG experiment)
  FetchContent_MakeAvailable(cpr)
  set(CPR "cpr::cpr")
endif()

#Get OpenSSL if needed
if(HTTPS)
  find_package(OpenSSL REQUIRED)
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads std::filesystem ${CPR})
```
