#if !defined(CACHE_H_FILE)
#define CACHE_H_FILE

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
    virtual std::optional<Ret> get(Args...) = 0;
    virtual void put(Args..., Ret) = 0;
  protected:
    string (*key)(Args...);
    string (*pickle)(Ret);
    Ret (*unpickle)(string);
};

#endif
