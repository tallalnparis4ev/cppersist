#include <optional>
#include <functional>
#include <iostream>
#include <unordered_map>
#include "../cacheimpl/memory/regcache.hpp"
#include "../cacheimpl/memory/lrucache.hpp"
#include "../utils/log.hpp"
using std::function;
using std::string;


template<typename T, typename Ret, typename... Args>
void PersistentMemoized<T,Ret,Args...>::printCaches(){
  log(this->primaryCache, this->secondaryCache);
}
template<typename Ret, typename... Args>
MemCache<Ret,Args...>* getMemoryCache(MemCacheType type,string (*key)(const Args&...),string (*pickle)(const Ret&),Ret (*unpickle)(const string&)){
  switch(type){
    case LRU_CACHE: return new LRUCache<Ret,Args...>(1000,key,pickle,unpickle);
    case REGULAR: return new RegCache<Ret,Args...>(key,pickle,unpickle);
  }
  return new RegCache<Ret,Args...>(key,pickle,unpickle);
}

//Destructor
template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...>::~PersistentMemoized(){
  deleteCaches();
}

//Constructor1
template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...>::PersistentMemoized(Cache<Ret,Args...>* cache){
  log("constructor1");
  this->primaryCache = cache;
  this->secondaryCache = NULL;
}

//Constructor2
template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...>::PersistentMemoized(MemCache<Ret,Args...>* primaryCache, 
  Cache<Ret,Args...>* secondaryCache){
  log("constructor2");
  this->primaryCache = primaryCache;
  this->secondaryCache = secondaryCache;
}

//Move Constructor
template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...>::PersistentMemoized(PersistentMemoized<T,Ret,Args...>&& rvalue){
  log("move constructor");
  this->primaryCache = rvalue.primaryCache;
  this->secondaryCache = rvalue.secondaryCache;
  rvalue.primaryCache = NULL;
  rvalue.secondaryCache = NULL;
}

//Move Assignment
template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...>& PersistentMemoized<T,Ret,Args...>::operator=(PersistentMemoized<T,Ret,Args...>&& rvalue){
  log("move assignment");
  deleteCaches();
  this->primaryCache = rvalue.primaryCache;
  this->secondaryCache = rvalue.secondaryCache;
  rvalue.primaryCache = NULL;
  rvalue.secondaryCache = NULL;
  return *this;
}

//Copy Constructor
template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...>::PersistentMemoized(const PersistentMemoized<T,Ret,Args...>& lvalue)
{
  log("copy constructor");
  this->primaryCache = lvalue.primaryCache->clone();
  this->secondaryCache = NULL;
  if(lvalue.secondaryCache != NULL) this->secondaryCache = lvalue.secondaryCache->clone();
}

//Copy Assignment
template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...>& PersistentMemoized<T,Ret,Args...>::operator=(const PersistentMemoized<T,Ret,Args...>& lvalue){
  log("copy assignment");
  if (this != &lvalue) { // no self-assignment
    deleteCaches();
    this->primaryCache = lvalue.primaryCache->clone();
    this->secondaryCache = NULL;
    if(lvalue.secondaryCache != NULL) this->secondaryCache = lvalue.secondaryCache->clone();
  }
  return *this;
}

template<typename T, typename Ret, typename ...Args>
void PersistentMemoized<T,Ret,Args...>::deleteCaches(){
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
