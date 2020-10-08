#include <optional>
#include <string>
#include <functional>
#include <iostream>
using std::function;
using std::string;

template<typename T, typename Ret, typename ...Args>
class Memoized: public T{
  public:
    Ret solve(Args... args) override {
      std::cout << "Modified" << std::endl;
      return T::solve(args...);
    } 
};

template<typename Ret, typename... Args>
class PersistentMemoizable
{
  public:
    virtual Ret solve(Args... args) = 0;
    template<typename T> 
    Memoized<T,Ret,Args...> getMemoized(T method){
      Memoized<T,Ret,Args...> memoized;
      return memoized;
    };
};
