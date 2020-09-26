#if !defined(CACHE_H_FILE)
#define CACHE_H_FILE
#include <optional>
#include <string>
#include <functional>
#include <iostream>
using std::function;
using std::string;
template <typename Key, typename Value> class Cache
{
  public:
    Cache(function<Value(string)>, function<string(Value)>);
    std::optional<Value> get(Key);
    void put(Key, Value);
  private:
    function<Value(string)> pickle;
    function<string(Value)> unpickle;
};

#include "cache.cpp"

#endif // MACRO
