#ifndef MEM_CACHE_H_FILE
#define MEM_CACHE_H_FILE
#include <optional>
#include <string>

#include "cache.hpp"
namespace cpst {
/**
 * This is an abstract class representing a cache that is stored
 * in-memory.
 */
template <typename Ret, typename... Args>
class MemCache : public Cache<Ret, Args...> {
 public:
  MemCache(std::string (*key)(Args...), std::string (*pickle)(Ret),
           Ret (*unpickle)(std::string)) {
    this->key = key;
    this->pickle = pickle;
    this->unpickle = unpickle;
  }
 protected:
  virtual std::optional<std::string> getFromCache(const std::string&) = 0;
};
}  // namespace cpst
#endif