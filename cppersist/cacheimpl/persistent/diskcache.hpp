#ifndef DISK_CACHE_H_FILE
#define DISK_CACHE_H_FILE

#include <optional>
#include <string>

#include "../../interfaces/percache.hpp"
namespace cpst {
/**
 * This is a class that represents a persistent cache which is stored in
 * the local filesystem.
 */
template <typename Ret, typename... Args>
class DiskCache : public PerCache<Ret, Args...> {
 public:
  using PerCache<Ret, Args...>::PerCache;
  DiskCache(string (*key)(Args...), string (*pickle)(Ret),
            Ret (*unpickle)(string), string (*hash)(string), string funcName);
  DiskCache<Ret, Args...>* clone();
  std::optional<Ret> get(const Args&... args);
  void put(const Args&... args, const Ret& value);
  void setLoc(string&);
 private:
  string makePathForKey(const string& key);
  string outputPath;
};
}  // namespace cpst
#include "diskcache.cpp"
#endif