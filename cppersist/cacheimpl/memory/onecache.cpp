#include <string>
#include <optional>
using std::optional;
using std::nullopt;
using std::string;

namespace cpst{

  template <typename Ret, typename ...Args> 
  OneCache<Ret,Args...>* OneCache<Ret,Args...>::clone(){
    return new OneCache<Ret,Args...>(this->key,this->pickle,this->unpickle);
  }

  template <typename Ret, typename ...Args> 
  OneCache<Ret,Args...>::OneCache(string (*key)(Args...),
    string (*pickle)(Ret),Ret (*unpickle)(string))
  {
    this->key = key;
    this->pickle = pickle;
    this->unpickle = unpickle;
  }

  template <typename Ret, typename ...Args> 
  std::optional<string> OneCache<Ret,Args...>::getFromCache(const string& key){
    if(key.compare(lastKey) == 0)
      return optional<string>{lastValue};
    return nullopt;
  }

  template <typename Ret, typename ...Args> 
  std::optional<Ret> OneCache<Ret,Args...>::get(const Args&... args) {
    string key = this->key(args...);
    std::optional<string> answer = getFromCache(key);
    if(!answer) return nullopt;
    return optional<Ret>{this->unpickle(answer.value())};
  }

  template <typename Ret, typename ...Args> 
  void OneCache<Ret,Args...>::put(const Args&... args, const Ret& value)  {
    lastKey = this->key(args...);
    lastValue = this->pickle(value);
  }
}