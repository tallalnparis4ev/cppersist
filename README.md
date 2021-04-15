# C++ Persistent Memoization : cppersist

## TLDR

Cppersist is a small framework which allows users to apply persistent memoization to a member function. Cppersist allows member functions to be memoized via a filesystem, MongoDB, or in-memory.

## Documentation

[![Documentation](https://img.shields.io/badge/docs-online-informational?style=for-the-badge&link=https://tallalnparis4ev.github.io/)](https://tallalnparis4ev.github.io/)  
The documentation of cppersist is found [here](https://tallalnparis4ev.github.io/). Navigate to the 'modules' tab for documentation on the user-visible parts of cppersist.

## Minimal Working Example
```console
~$ git clone https://github.com/tallalnparis4ev/cppersist.git
~$ cd ./cppersist/demo
~/cppersist/demo$ ./build_demo
~/cppersist/demo$ ./demo
```
You should see the output "2" twice. You should also have a directory called "persist" that is populated with entries.
## Example C++ File Using cppersist
Here is an example of how cppersist is used:

```c++
#include <cppersist/local.hpp> //if you need to memoize with your filesystem
#include <cppersist/mongo.hpp> //if you need to memoize with MongoDB
#include <cppersist/memory.hpp> //if you need to memoize in-memory
#include <iostream>

//The class containing the function you want to memoize
class FibonacciSolver: public cpst::Memoizable<int, int>{ //<--- these templates must correspond to 
//<return type of function to memoize, comma separated list of input types of function to memoize>
  public:
    int solve(int n) override { //the function you want to memoize - must be called "solve"!
      if(n<=1) return n;
      return solve(n-1) + solve(n-2);
    }
};

//Serialization function
std::string intostr(int x){
  return std::to_string(x);
}
//Deserialization function
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

## Example CMake File Building the Above File
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
if(MONGO)
  FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/tallalnparis4ev/cpr.git GIT_TAG stable)
  FetchContent_MakeAvailable(cpr)
  set(CPR "cpr::cpr")
endif()
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads std::filesystem ${CPR})
```
## Build - only for filesystem cache
```
cmake .
make
```

## Build - for MongoDB cache
Note: not all systems will require you to pass -DOPENSSL_ROOT_DIR. This was a requirment on macOS Catalina 10.15.6, and was solved by passing in the path to OpenSSL which will look like "/usr/local/opt/openssl". However, on CentOS 8 there was no need to pass in anything.
```
cmake -DMONGO=true -DOPENSSL_ROOT_DIR=/path/to/openssl .
make
```
## Setting up the MongoDB server
```console
~/cppersist/$ cd mongo_server
~/cppersist/mongo_server$ python3 setup.py install
~/cppersist/mongo_server$ python3 run.py [desried port number]
```
This will run the MongoDB server at localhost:[desired port number]. The default port number is 5000, which will be used if a port number isn't passed in. The MongoDB server is configured by the settings.py file. 

By default the MongoDB server communicates to a single MongoDB instance which should run at localhost:27017. You are responsible for setting up this MongoDB instance in any way you would like.

## Running Tests
```console
~$ cd ./cppersist/test
~/cppersist/test$ ./build_tests /path/to/openssl
~/cppersist/test$ ./run_tests
```
Note: again, not all systems will require you to pass /path/to/openssl. This was a requirment on macOS Catalina 10.15.6, and was solved by passing in the path to OpenSSL which will look like "/usr/local/opt/openssl". However, on CentOS 8 there was no need to pass in anything.

Additionally, for the MongoDB tests to pass you must setup two MongoDB servers - one listening at port 5000 and the other at port 5001. The MongoDB instance they communicate with must not have any entries in the collection "persist" of the database "memoisation".
