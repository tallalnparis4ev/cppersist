#include <optional>
#include <unordered_map>
#include <list>
#include <string>
using std::optional;
using std::nullopt;
using std::function;
using std::string;

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

#include <iostream>
using namespace std;
template <typename Ret, typename ...Args> 
void LRUCache<Ret,Args...>::print(){
  for ( const std::pair<const string&, const string&> &pair : cache ) {
    string key = pair.first;
    string value = pair.second;
    std::cout << pair.first << " , " << value << std::endl;
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
  cout << "Putting " << key << ", " << value << endl;
  cout << "cur size " << recentlyUsed.size() << endl;
  if(recentlyUsed.size() == cacheSize){
    string oldestKey = recentlyUsed.back();
    cout << "removing " << oldestKey << endl;
    recentlyUsed.pop_back();
    positionCache.erase(oldestKey);
    cache.erase(oldestKey);
  }
  recentlyUsed.push_front(key);
  positionCache[key] = recentlyUsed.begin();
  cache[key] = value;
}