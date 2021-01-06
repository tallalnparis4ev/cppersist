#include <string>
#include <optional>
using std::optional;
using std::nullopt;
using std::function;
using std::string;

template <typename Ret, typename ...Args> 
RegCache<Ret,Args...>::RegCache(string (*key)(const Args&...),
  string (*pickle)(const Ret&),Ret (*unpickle)(const string&))
{
  this->key = key;
  this->pickle = pickle;
  this->unpickle = unpickle;
}

template <typename Ret, typename ...Args> 
const std::map<string,string>& RegCache<Ret,Args...>::getContents(){
  return this->cache;
}

template <typename Ret, typename ...Args> 
std::optional<string> RegCache<Ret,Args...>::getFromCache(const string& key){
  if(!this->cache.contains(key)) return nullopt;
  return optional<string>{this->cache[key]};
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