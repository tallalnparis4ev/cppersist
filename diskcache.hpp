#include "cache.hpp"
#include <optional>

template <typename Ret, typename ...Args> 
class DiskCache : public Cache<Ret,Args...>{
  public:
    DiskCache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string))
    {
      this->key = key;
      this->pickle = pickle;
      this->unpickle = unpickle;
    }
    std::optional<Ret> get(Args... args);
    void put(Args... args, Ret value);
};

#if !defined(DISKCACHE_H_FILE)
#define DISKCACHE_H_FILE
#include "diskcache.cpp"
#endif
