#pragma once 
#include <optional>
#include <string>
using std::string;
template <typename Ret, typename ...Args> 
class Cache
{
  public:
    virtual std::optional<Ret> get(Args...) = 0;
    virtual void put(Args..., Ret) = 0;
  protected:
    std::string (*key)(Args...);
    std::string (*pickle)(Ret);
    Ret (*unpickle)(string);
    std::string funcName;
};
