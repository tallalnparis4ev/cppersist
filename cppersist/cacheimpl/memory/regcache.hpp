#ifndef REG_CACHE_H_FILE
#define REG_CACHE_H_FILE

#include "../../interfaces/memcache.hpp"
#include <optional>
#include <unordered_map>
#include <string>

template <typename Ret, typename ...Args> 
class RegCache : public MemCache<Ret,Args...>{
  public:
    RegCache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string));
    RegCache* clone();
    std::optional<Ret> get(const Args&... args);
    void put(const Args&... args, const Ret& value);
    void put(const std::string&, const std::string&);
    void populateCache(Cache<Ret,Args...>*);
  private:
    std::optional<string> getFromCache(const string&);
    std::unordered_map<string,string> cache;
};

#include "regcache.cpp"
#endif