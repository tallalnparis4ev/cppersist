#include <optional>
#include <unordered_map>
#include <list>
#include <string>
#include "../../utils/log.hpp"
using std::optional;
using std::nullopt;
using std::function;
using std::string;

template <typename Ret, typename ...Args> 
LRUCache<Ret,Args...>* LRUCache<Ret,Args...>::clone(){
  return new LRUCache<Ret,Args...>(this->cacheSize,this->key,this->pickle,this->unpickle);
}

template <typename Ret, typename ...Args> 
LRUCache<Ret,Args...>::LRUCache(int cacheSize, string (*key)(const Args&...),
  string (*pickle)(const Ret&),Ret (*unpickle)(const string&))
{
  this->cacheSize = cacheSize;
  this->key = key;
  this->pickle = pickle;
  this->unpickle = unpickle;
}

template <typename Ret, typename ...Args> 
void LRUCache<Ret,Args...>::populateCache(Cache<Ret,Args...>* secondaryCache){
  for ( const std::pair<const string&, const string&> &pair : cache ) {
    string key = pair.first;
    string value = pair.second;
    secondaryCache->put(key,value);
  } 
}

template <typename Ret, typename ...Args> 
void LRUCache<Ret,Args...>::print(){
  for ( const std::pair<const string&, const string&> &pair : cache ) {
    string key = pair.first;
    string value = pair.second;
    log(key,value);
  } 
}


template <typename Ret, typename ...Args> 
std::optional<string> LRUCache<Ret,Args...>::getFromCache(const string& key){
  if(!this->cache.contains(key)) return nullopt;
  return optional<string>{cache[key]};
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
  string valueStr = this->key(value);
  put(key,valueStr);
}

template <typename Ret, typename ...Args> 
void LRUCache<Ret,Args...>::put(const string& key, const string& value)  {
  if(recentlyUsed.size() == cacheSize){
    string oldestKey = recentlyUsed.back();
    recentlyUsed.pop_back();
    positionCache.erase(oldestKey);
    cache.erase(oldestKey);
  }
  recentlyUsed.push_front(key);
  positionCache[key] = recentlyUsed.begin();
  cache[key] = value;
}