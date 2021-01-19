#include <optional>
#include <unordered_map>
#include <list>
#include <string>
#include "../../utils/log.hpp"
using std::optional;
using std::nullopt;
using std::function;
using std::string;
namespace cpst{
  template <typename Ret, typename ...Args> 
  LRUCache<Ret,Args...>* LRUCache<Ret,Args...>::clone(){
    return new LRUCache<Ret,Args...>(this->cacheSize,this->key,this->pickle,this->unpickle);
  }

  template <typename Ret, typename ...Args> 
  LRUCache<Ret,Args...>::LRUCache(int cacheSize, string (*key)(Args...),
    string (*pickle)(Ret),Ret (*unpickle)(string))
  {
    this->cacheSize = cacheSize;
    this->key = key;
    this->pickle = pickle;
    this->unpickle = unpickle;
  }

  template <typename Ret, typename ...Args> 
  std::optional<string> LRUCache<Ret,Args...>::getFromCache(const string& key){
    std::unordered_map<string,string>::iterator iter = cache.find(key);
    if(iter != cache.end()) //found it
      return optional<string>{iter->second};
    return nullopt;
  }

  template <typename Ret, typename ...Args> 
  std::optional<Ret> LRUCache<Ret,Args...>::get(const Args&... args) {
    string key = this->key(args...);
    std::optional<string> answer = getFromCache(key);

    if(!answer) return nullopt;
    
    recentlyUsed.erase(positionCache[key]);  
    recentlyUsed.push_front(key);
    positionCache[key] = recentlyUsed.begin();
    
    return optional<Ret>{this->unpickle(answer.value())};
  }

  template <typename Ret, typename ...Args> 
  void LRUCache<Ret,Args...>::put(const Args&... args, const Ret& value)  {
    string key = this->key(args...);
    if(recentlyUsed.size() == cacheSize){
      string oldestKey = recentlyUsed.back();
      recentlyUsed.pop_back();
      positionCache.erase(oldestKey);
      cache.erase(oldestKey);
    }
    recentlyUsed.push_front(key);
    positionCache[key] = recentlyUsed.begin();
    cache[key] = this->pickle(value);
  }
}