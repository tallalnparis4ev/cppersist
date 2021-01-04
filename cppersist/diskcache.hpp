#include "cache.hpp"
#include <optional>

template <typename Ret, typename ...Args> 
class DiskCache : public Cache<Ret,Args...>{
  public:
    DiskCache(string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&), const string& funcName);
    std::optional<Ret> get(const Args&... args);
    void put(const Args&... args, const Ret& value);
  private:
    string makePathForKey(const string& key);
    string outputPath;
};

#include "diskcache.cpp"
