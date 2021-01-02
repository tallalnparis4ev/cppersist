#ifndef CACHE_H_FILE
#define CACHE_H_FILE

#include <optional>
#include <string>
template <typename Ret, typename ...Args> 
class Cache
{
  public:
    virtual std::optional<Ret> get(Args...) = 0;
    virtual void put(Args..., Ret) = 0;
  protected:
    std::string (*key)(Args...);
    std::string (*pickle)(Ret);
    Ret (*unpickle)(std::string);
    std::string funcName;
};

#endif