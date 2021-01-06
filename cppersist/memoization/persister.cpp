#include <optional>
#include <functional>
#include <iostream>
#include <unordered_map>
using std::function;
using std::string;

template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...>::PersistentMemoized(Cache<Ret,Args...>* cache){
  this->primaryCache = cache;
  this->secondaryCache = NULL;
}

template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...>::PersistentMemoized(MemCache<Ret,Args...>* primaryCache, 
  Cache<Ret,Args...>* secondaryCache)
{
  this->primaryCache = primaryCache;
  this->secondaryCache = secondaryCache;
}

template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...>::~PersistentMemoized(){
  if(this->secondaryCache != NULL){
    MemCache<Ret,Args...>* primaryCacheCast = (MemCache<Ret,Args...>*) this->primaryCache;
    primaryCacheCast->populateCache(this->secondaryCache);
    delete this->secondaryCache;
  }
  delete this->primaryCache;
}

template<typename T, typename Ret, typename ...Args>
Ret PersistentMemoized<T,Ret,Args...>::operator()(Args... args) {
  return solve(args...);
}

template<typename T, typename Ret, typename ...Args>
Ret PersistentMemoized<T,Ret,Args...>::solve(Args... args){
  std::optional<Ret> answer = primaryCache->get(args...);  
  if(answer){
    std::cout << "CACHE HIT" << std::endl;
    return answer.value();
  }
  if(this->secondaryCache != NULL){
    answer = secondaryCache->get(args...);
    if(answer){
      std::cout << "SECONDARY CACHE HIT" << std::endl;
      return answer.value();
    }
  }

  Ret realAnswer = T::solve(args...);
  primaryCache->put(args..., realAnswer);
  return realAnswer;  
}
