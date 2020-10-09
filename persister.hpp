#include <optional>
#include <string>
#include <functional>
#include <iostream>
using std::function;
using std::string;

template<typename T, typename Ret, typename ...Args>
class PersistentMemoized: public T{
  public:
    Ret operator()(Args... args) {
      return solve(args...);
    }
  private:
    Ret solve(Args... args) {
      std::cout << "Modified" << std::endl;
      return T::solve(args...);
    }
};

template<typename Ret, typename... Args>
class PersistentMemoizable
{
  public:
    virtual Ret solve(Args... args) = 0;
};

class Persister
{
  public:
    template<typename T, typename Ret, typename... Args> 
    static PersistentMemoized<T,Ret,Args...> getMemoizedObj(PersistentMemoizable<Ret, Args...>& object){
      PersistentMemoized<T,Ret,Args...> memoized;
      return memoized;
    }
};