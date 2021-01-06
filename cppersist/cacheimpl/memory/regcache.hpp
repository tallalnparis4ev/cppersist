#ifndef REG_CACHE_H_FILE
#define REG_CACHE_H_FILE

#include "../interfaces/memcache.hpp"
#include <optional>
#include <unordered_map>
#include <string>

template <typename Ret, typename ...Args> 
class RegCache : public MemCache<Ret,Args...>{
  public:
    RegCache(string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&));
    std::optional<Ret> get(const Args&... args);
    void put(const Args&... args, const Ret& value);
    void put(const std::string&, const std::string&);
    const std::unordered_map<string,string>& getContents();
  private:
    std::optional<string> getFromCache(const string&);
    std::unordered_map<string,string> cache;
};

#include "regcache.cpp"
#endif