#include <list>
#include <optional>
#include <string>
#include <unordered_map>

#include "../../utils/log.hpp"
using std::function;
using std::nullopt;
using std::optional;
using std::string;
namespace cpst {
template <typename Ret, typename... Args>
LRUCache<Ret, Args...>* LRUCache<Ret, Args...>::clone() {
  return new LRUCache<Ret, Args...>(this->cacheSize, this->key, this->pickle,
                                    this->unpickle);
}

// Constructor
template <typename Ret, typename... Args>
LRUCache<Ret, Args...>::LRUCache(int cacheSize, string (*key)(Args...),
                                 string (*pickle)(Ret),
                                 Ret (*unpickle)(string)) : MemCache<Ret,Args...>(key,pickle,unpickle) {
  this->cacheSize = cacheSize;
}

template <typename Ret, typename... Args>
std::optional<string> LRUCache<Ret, Args...>::getFromCache(const string& key) {
  std::unordered_map<string, string>::iterator iter = cache.find(key);
  if (iter != cache.end())                  // found it in the map
    return optional<string>{iter->second};  // return the value
  return nullopt;
}

template <typename Ret, typename... Args>
std::optional<Ret> LRUCache<Ret, Args...>::get(const Args&... args) {
  // Create a string key from the arguments
  string key = this->key(args...);
  // Check if the key exists in the cache
  std::optional<string> answer = getFromCache(key);

  if (!answer) return nullopt;  // Key doesn't exist in cache

  // Key does exist in cache, must update this key to be the most recently
  // accessed key
  recentlyUsed.erase(positionCache[key]);
  recentlyUsed.push_front(key);
  positionCache[key] = recentlyUsed.begin();

  // Return the value corresponding to the key
  return optional<Ret>{this->unpickle(answer.value())};
}

template <typename Ret, typename... Args>
void LRUCache<Ret, Args...>::put(const Args&... args, const Ret& value) {
  // Create a string key from the arguments
  string key = this->key(args...);

  if (recentlyUsed.size() == cacheSize) {  // The LRU is at maximum size
    // Find the least recently used key
    string oldestKey = recentlyUsed.back();
    recentlyUsed.pop_back();         // Remove least recently used key
    positionCache.erase(oldestKey);  // Remove least recently used key
    cache.erase(oldestKey);          // Remove least recently used key
  }

  // The key to be added becomes the most recently used
  recentlyUsed.push_front(key);
  // Store the iterator correpsonding to the key in the 'recentlyUsed'
  // list, so this node can be removed in O(1) in the future.
  positionCache[key] = recentlyUsed.begin();
  cache[key] = this->pickle(value);  // Store value in cache
}
}  // namespace cpst