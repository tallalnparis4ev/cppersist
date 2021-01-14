#include "../local.hpp"
#include <vector>
#include "../utils/log.hpp"
using namespace cpst;
using namespace std;

class ToyExample : public PersistentMemoizable<string, string>{
  public:
    string solve(string arg) override {
      return arg;
    };
};

string key(string result){
  return result;
}

int main(int argc, char const *argv[])
{
  PersistentMemoized toyExample = getLocalMemoizedObj<ToyExample,string,string>(key,key,key);
  /* code */
  return 0;
}