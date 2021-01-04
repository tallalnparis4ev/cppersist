# C++ Persistent Memoization : cppersist

## TLDR

cppersist is a small library which allows user's to convert member functions to a memoized equivalent. cppersist allows member functions can be memoized via disk storage or mongo collections.

Here is how cppersist is used:

```c++
#include <cppersist/disk.h>
#include <cppersist/mongo.h>

//The class to be memoized - must implement a solve method
class FibonacciSolver: public PersistentMemoizable<int, int>{
  public:
    int solve(int n) override {...}
};

int strtoi(string x){...}
string intostr(int x){...}
string keymaker(int x){...}

int main(int argc, char** argv) {
  PersistentMemoized memoizedFib = getLocalMemoizedObj<DFSSolver>(keymaker,intostr,strtoi); //disk cache
  PersistentMemoized memoizedFib = getMongoMemoizedObj<DFSSolver>(keymaker,intostr,strtoi); //mongo cache
  int fib = memoizedFib(...);
}
```
## Usage
To build your project, utilising cppersist, via CMake:
```cmake
include(FetchContent)
FetchContent_Declare(test GIT_REPOSITORY https://github.com/tallalnparis4ev/dnt GIT_TAG 1.0)  
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG c8d33915dbd88ad6c92b258869b03aba06587ff9) 
FetchContent_MakeAvailable(cppersist cpr)
include_directories(${test_SOURCE_DIR})
target_link_libraries(cppersist PRIVATE cpr::cpr)
```

However, if you're not using the mongo cache, the following CMake suffices (and it will be faster because no requests library is required):
```cmake
include(FetchContent)
FetchContent_Declare(test GIT_REPOSITORY https://github.com/tallalnparis4ev/dnt GIT_TAG 1.0)  
FetchContent_MakeAvailable(cppersist)
include_directories(${test_SOURCE_DIR})
```
