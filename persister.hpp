#include <optional>
#include <string>
#include <functional>
#include <iostream>
#include "cache.hpp"
using std::function;
using std::string;

template<typename T, typename Ret, typename ...Args>
class PersistentMemoized: public T{
  public:
    PersistentMemoized(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string)):
    persistentCache(Cache(key,pickle,unpickle))
    {}
    Ret operator()(Args... args) {
      return solve(args...);
    }
  private:
    Cache<Ret,Args...> persistentCache;
    Ret solve(Args... args) {
      optional<Ret> answer = persistentCache.get(args...);  
      if(answer){
        std::cout << "inside for" << std::endl;
        return answer.value();
      }
      Ret realAnswer = T::solve(args...);
      persistentCache.put(args..., realAnswer);
      return realAnswer;
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
    static PersistentMemoized<T,Ret,Args...> getMemoizedObj(PersistentMemoizable<Ret, Args...>& object,
    string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string)){
      PersistentMemoized<T,Ret,Args...> memoized(key,pickle,unpickle);
      return memoized;
    }
};