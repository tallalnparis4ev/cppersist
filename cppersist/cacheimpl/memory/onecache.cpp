#include <optional>
#include <string>
using std::nullopt;
using std::optional;
using std::string;

namespace cpst {

template <typename Ret, typename... Args>
OneCache<Ret, Args...>* OneCache<Ret, Args...>::clone() {
  return new OneCache<Ret, Args...>(this->key, this->pickle, this->unpickle);
}

template <typename Ret, typename... Args>
std::optional<string> OneCache<Ret, Args...>::getFromCache(const string& key) {
  // The previous cache access is the same as this one
  if (key.compare(lastKey) == 0)
    return optional<string>{lastValue};  // So return the previous value
  // Otherwise, this key is not stored
  return nullopt;
}

template <typename Ret, typename... Args>
std::optional<Ret> OneCache<Ret, Args...>::get(const Args&... args) {
  // Create a string key from the arguments
  string key = this->key(args...);
  // Check if the key exists in the cache
  std::optional<string> answer = getFromCache(key);
  if (!answer) return nullopt;  // Key doesn't exist in cache

  // Key does exist in cache so return its corresponding value
  return optional<Ret>{this->unpickle(answer.value())};
}

template <typename Ret, typename... Args>
void OneCache<Ret, Args...>::put(const Args&... args, const Ret& value) {
  // This cache only stores the previous access, therefore we only need one
  // key,value pair
  lastKey = this->key(args...);
  lastValue = this->pickle(value);
}
}  // namespace cpst