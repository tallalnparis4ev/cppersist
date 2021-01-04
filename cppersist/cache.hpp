#ifndef CACHE_H_FILE
#define CACHE_H_FILE

#include <optional>
#include <string>
template <typename Ret, typename ...Args> 
class Cache
{
  public:
    virtual std::optional<Ret> get(const Args&...) = 0;
    virtual void put(const Args&..., const Ret&) = 0;
    virtual ~Cache() {};
  protected:
    std::string (*key)(const Args&...);
    std::string (*pickle)(const Ret&);
    Ret (*unpickle)(const std::string&);
    std::string funcName;
};

#endif