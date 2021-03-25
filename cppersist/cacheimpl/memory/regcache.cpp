#include <optional>
#include <string>
using std::nullopt;
using std::optional;
using std::string;

namespace cpst {

template <typename Ret, typename... Args>
RegCache<Ret, Args...>* RegCache<Ret, Args...>::clone() {
  return new RegCache<Ret, Args...>(this->key, this->pickle, this->unpickle);
}

template <typename Ret, typename... Args>
std::optional<string> RegCache<Ret, Args...>::getFromCache(const string& key) {
  std::unordered_map<string, string>::iterator iter = cache.find(key);
  if (iter != cache.end())                  // Found the key in the cache
    return optional<string>{iter->second};  // So return its value
  // Otherwise, the key doesn't exist in the cache
  return nullopt;
}

template <typename Ret, typename... Args>
std::optional<Ret> RegCache<Ret, Args...>::get(const Args&... args) {
  // Create a string key from the arguments
  string key = this->key(args...);
  // Check if the key exists in the cache
  std::optional<string> answer = getFromCache(key);
  if (!answer) return nullopt;  // Key doesn't exist in cache

  // Key does exist in the cache, return its corresponding value
  return optional<Ret>{this->unpickle(answer.value())};
}

template <typename Ret, typename... Args>
void RegCache<Ret, Args...>::put(const Args&... args, const Ret& value) {
  string key = this->key(args...);
  // The underlying cache is an unordered_map<string,string>, so we can
  // add the entry (key(args...), pickle(value))
  this->cache[key] = this->pickle(value);
}
}  // namespace cpst