#ifndef ONE_CACHE_H_FILE
#define ONE_CACHE_H_FILE

#include <optional>
#include <string>

#include "../../interfaces/memcache.hpp"
namespace cpst {
/**
 * This is a class that represents an in-memory cache which only stores
 * the most recent key/value pair.
 */
template <typename Ret, typename... Args>
class OneCache : public MemCache<Ret, Args...> {
 public:
  OneCache(std::string (*key)(Args...), std::string (*pickle)(Ret),
           Ret (*unpickle)(std::string)) : MemCache<Ret,Args...>(key,pickle,unpickle) {}           
  OneCache* clone();
  std::optional<Ret> get(const Args&... args);
  void put(const Args&... args, const Ret& value);

 private:
  std::optional<std::string> getFromCache(const std::string&);
  std::string lastKey;
  std::string lastValue;
};
}  // namespace cpst
#include "onecache.cpp"
#endif