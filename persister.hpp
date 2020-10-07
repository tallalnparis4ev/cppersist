#include <optional>
#include <string>
#include <functional>
#include <iostream>
using std::function;
using std::string;
template<typename Ret, typename... Args>
class Persister
{
  public:
    virtual Ret solve(Args... args) = 0;
    int getAnswer(int x){
      std::cout << "Modified" << std::endl;
      return solve(x);
    };
};


