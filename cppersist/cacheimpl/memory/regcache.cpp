#include <string>
#include <optional>
using std::optional;
using std::nullopt;
using std::function;
using std::string;


template <typename Ret, typename ...Args> 
RegCache<Ret,Args...>* RegCache<Ret,Args...>::clone(){
  return new RegCache<Ret,Args...>(this->key,this->pickle,this->unpickle);
}

template <typename Ret, typename ...Args> 
RegCache<Ret,Args...>::RegCache(string (*key)(const Args&...),
  string (*pickle)(const Ret&),Ret (*unpickle)(const string&))
{
  this->key = key;
  this->pickle = pickle;
  this->unpickle = unpickle;
}

template <typename Ret, typename ...Args> 
void RegCache<Ret,Args...>::populateCache(Cache<Ret,Args...>* secondaryCache){
  for ( const std::pair<const string&, const string&> &pair : this->cache ) {
    secondaryCache->put(pair.first,pair.second);
  } 
}

template <typename Ret, typename ...Args> 
std::optional<string> RegCache<Ret,Args...>::getFromCache(const string& key){
  std::unordered_map<string,string>::iterator iter = cache.find(key);
  if(iter != cache.end()) //found it
    return optional<string>{iter->second};
  return nullopt;
}

template <typename Ret, typename ...Args> 
std::optional<Ret> RegCache<Ret,Args...>::get(const Args&... args) {
  string key = this->key(args...);
  std::optional<string> answer = getFromCache(key);
  if(!answer) return nullopt;
  return optional<Ret>{this->unpickle(answer.value())};
}

template <typename Ret, typename ...Args> 
void RegCache<Ret,Args...>::put(const Args&... args, const Ret& value)  {
  string key = this->key(args...);
  string valueStr = this->key(value);
  put(key,valueStr);
}

template <typename Ret, typename ...Args> 
void RegCache<Ret,Args...>::put(const string& key, const string& value)  {
  this->cache[key] = value;
}