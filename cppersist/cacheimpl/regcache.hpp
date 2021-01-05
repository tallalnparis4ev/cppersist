#ifndef REG_CACHE_H_FILE
#define REG_CACHE_H_FILE

#include "../interfaces/memcache.hpp"
#include <optional>
#include <map>

template <typename Ret, typename ...Args> 
class RegCache : public MemCache<Ret,Args...>{
  public:
    RegCache(string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&));
    std::optional<Ret> get(const Args&... args);
    void put(const Args&... args, const Ret& value);
    void put(const std::string&, const std::string&);
    const std::map<string,string>& getContents();
  private:
    std::optional<string> getFromCache(string);
    std::map<string,string> cache;
    string makePathForKey(const string& key);
    string outputPath;
};

#include "regcache.cpp"
#endif