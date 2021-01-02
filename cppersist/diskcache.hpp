#include "cache.hpp"
#include <optional>

template <typename Ret, typename ...Args> 
class DiskCache : public Cache<Ret,Args...>{
  public:
    DiskCache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string funcName);
    std::optional<Ret> get(Args... args);
    void put(Args... args, Ret value);
  private:
    string makePathForKey(string key);
    string outputPath;
};

#include "diskcache.cpp"
