#include <optional>
#include <string>
#include <functional>
#include <iostream>
using std::function;
using std::string;
template <typename Ret, typename ...Args> 
class Cache
{
  public:
    Cache();
    Cache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string));
    std::optional<Ret> get(Args...);
    void put(Args..., Ret);
  private:
    string (*key)(Args...);
    // function<string(Args...)> key;
    string (*pickle)(Ret);
    // function<string(Ret)> pickle;
    Ret (*unpickle)(string);
    // function<Ret(string)> unpickle;
};

#if !defined(CACHE_H_FILE)
#define CACHE_H_FILE

#include "cache.cpp"

#endif
