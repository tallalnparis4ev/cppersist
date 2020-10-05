#include <optional>
#include <string>
#include <functional>
#include <iostream>
using std::function;
using std::string;
template <typename Key, typename Value> 
class Cache
{
  public:
    Cache();
    Cache(function<string(Key)>, function<string(Value)>, function<Value(string)>);
    std::optional<Value> get(Key);
    void put(Key, Value);
  private:
    function<string(Key)> key;
    function<string(Value)> pickle;
    function<Value(string)> unpickle;
};

#if !defined(CACHE_H_FILE)
#define CACHE_H_FILE

#include "cache.cpp"

#endif
