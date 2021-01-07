#ifndef DISK_CACHE_H_FILE
#define DISK_CACHE_H_FILE

#include "../../interfaces/percache.hpp"
#include <optional>
#include <string>

template <typename Ret, typename ...Args> 
class DiskCache : public PerCache<Ret,Args...>{
  public:
    DiskCache<Ret,Args...>* clone();
    DiskCache(string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string), string funcName);
    std::optional<Ret> get(const Args&... args);
    void put(const Args&... args, const Ret& value);
    void put(const string& key, const string& value);
  private:
    string makePathForKey(const string& key);
    string outputPath;
};

#include "diskcache.cpp"
#endif