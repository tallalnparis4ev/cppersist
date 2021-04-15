#ifndef REG_CACHE_H_FILE
#define REG_CACHE_H_FILE

#include <optional>
#include <string>
#include <unordered_map>

#include "../../interfaces/memcache.hpp"
namespace cpst {
/**
 * This is a class that represents an in-memory cache which has a limit
 * on the number of entries it can store. When that limit is reached,
 * entries are removed randomly from the cache.
 */
template <typename Ret, typename... Args>
class RegCache : public MemCache<Ret, Args...> {
 public:
  RegCache(int size, std::string (*key)(Args...), std::string (*pickle)(Ret),
           Ret (*unpickle)(std::string))
      : MemCache<Ret, Args...>(size, key, pickle, unpickle) {}
  RegCache* clone();
  std::optional<Ret> get(const Args&... args);
  void put(const Args&... args, const Ret& value);

 private:
  std::optional<string> getFromCache(const string&);
  std::unordered_map<string, string> cache;
};
}  // namespace cpst
#include "regcache.cpp"
#endif