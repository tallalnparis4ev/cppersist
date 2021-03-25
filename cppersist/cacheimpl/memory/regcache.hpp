#ifndef REG_CACHE_H_FILE
#define REG_CACHE_H_FILE

#include <optional>
#include <string>
#include <unordered_map>

#include "../../interfaces/memcache.hpp"
namespace cpst {
/**
 * This is a class that represents an in-memory cache with no bound on the
 * number of entries it has.
 */
template <typename Ret, typename... Args>
class RegCache : public MemCache<Ret, Args...> {
 public:
  using MemCache<Ret, Args...>::MemCache;
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