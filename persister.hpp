#include <optional>
#include <string>
#include <functional>
#include <iostream>
#include "cache.hpp"
#include "diskcache.hpp"
#include "mongodbcache.hpp"
using std::function;
using std::string;

template<typename T, typename Ret, typename ...Args>
class PersistentMemoized: public T{
  public:
    PersistentMemoized(Cache<Ret,Args...>* cache):persistentCache(cache)
    {}
    ~PersistentMemoized(){
      free(persistentCache);
    }
    Ret operator()(Args... args) {
      return solve(args...);
    }
  private:
    Cache<Ret,Args...>* persistentCache;
    Ret solve(Args... args) {
      optional<Ret> answer = persistentCache->get(args...);  
      if(answer){
        std::cout << "inside for" << std::endl;
        return answer.value();
      }
      Ret realAnswer = T::solve(args...);
      persistentCache->put(args..., realAnswer);
      return realAnswer;
    }
};

template<typename Ret, typename... Args>
class PersistentMemoizable
{
  public:
    virtual Ret solve(Args... args) = 0;
};

class Persister
{
  public:
    template<typename T, typename Ret, typename... Args> 
    static PersistentMemoized<T,Ret,Args...> getLocalMemoizedObj(PersistentMemoizable<Ret, Args...>& object,
    string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string)){
      DiskCache<Ret,Args...>* diskCache = new DiskCache<Ret,Args...>(key,pickle,unpickle);
      PersistentMemoized<T,Ret,Args...> memoized(diskCache);
      return memoized;
    }
    template<typename T, typename Ret, typename... Args> 
    static PersistentMemoized<T,Ret,Args...> getMongoMemoizedObj(PersistentMemoizable<Ret, Args...>& object,
    string (*key)(Args...),string (*pickle)(Ret),Ret (*unpickle)(string)){
      MongoDBCache<Ret,Args...>* mongoCache = new MongoDBCache<Ret,Args...>(key,pickle,unpickle);
      PersistentMemoized<T,Ret,Args...> memoized(mongoCache);
      return memoized;
    }
};