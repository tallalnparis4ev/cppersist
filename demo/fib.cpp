#include "cppersist/local.hpp"
#include "cppersist/mongo.hpp"
using namespace cpst;
using namespace std;
class FibSolver : Memoizable<int, int>{
  public:
    int solve(int n) override {
      std::cout << "Original" << std::endl;
      if(n==0) return 0;
      if(n==1) return 1;
      return solve(n-1) + solve(n-2);
    };
};

int strtoi(string x){
  return std::stoi(x);
}
string intostr(int x){
  return std::to_string(x);
}

string fibKeyMaker(int x){
  return std::to_string(x);
}

string hashKey(string key){
  return key;
}
int main(int argc, char const *argv[])
{
  auto localFib = getLocalMemoizedObj<FibSolver>(
    fibKeyMaker,intostr,strtoi,"fib",hashKey
  );

  auto mongoFib = getMongoMemoizedObj<FibSolver>(
    fibKeyMaker,intostr,strtoi,"localhost:5000","fib",hashKey
  );
  localFib(6);
  mongoFib(6);
  return 0;
}
