#ifndef PERSISTER_H_FILE
#define PERSISTER_H_FILE

#include <string>
#include "cache.hpp"
using std::string;

template<typename T, typename Ret, typename ...Args>
class PersistentMemoized: public T{
  public:
    PersistentMemoized(Cache<Ret,Args...>* cache);
    ~PersistentMemoized();
    Ret operator()(Args... args);
  private:
    Cache<Ret,Args...>* persistentCache;
    virtual Ret solve(Args... args);
};

template<typename Ret, typename... Args>
class PersistentMemoizable
{
  public:
    virtual Ret solve(Args... args) = 0;
};

#include "persister.cpp"
#endif