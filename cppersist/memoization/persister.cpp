#include <optional>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <future>
#include <mutex>
#include <chrono>
#include "../cacheimpl/memory/regcache.hpp"
#include "../cacheimpl/memory/lrucache.hpp"
#include "../cacheimpl/memory/onecache.hpp"
#include "../utils/log.hpp"
using std::function;
using std::string;
using namespace std::chrono;

namespace cpst{

  template<typename T, typename Ret, typename ...Args>
  void PersistentMemoized<T,Ret,Args...>::resetMetrics(){
    cacheHits = 0;
    cacheMisses = 0;
  }

  template<typename T, typename Ret, typename... Args>
  void PersistentMemoized<T,Ret,Args...>::nullFields(){
    this->primaryCache = NULL;
    this->secondaryCache = NULL;
  }

  template<typename T, typename Ret, typename... Args>
  void PersistentMemoized<T,Ret,Args...>::setMemoryCache(MemCacheType type){
    setMemoryCache(type,1000);
  }

  template<typename T, typename Ret, typename... Args>
  void PersistentMemoized<T,Ret,Args...>::setMemoryCache(MemCacheType type, int size){
    if(this->secondaryCache == NULL){
      if(this->primaryCache == NULL) return;
      this->secondaryCache = this->primaryCache;
    }
    else if(this->primaryCache != NULL) delete this->primaryCache;

    memCacheType = type;
    
    auto key = this->secondaryCache->getKey();
    auto pickle = this->secondaryCache->getPickle();
    auto unpickle = this->secondaryCache->getUnpickle();
    
    switch(type){
      case LRU_CACHE: primaryCache = new LRUCache<Ret,Args...>(size,key,pickle,unpickle); break;
      case REGULAR: primaryCache = new RegCache<Ret,Args...>(key,pickle,unpickle); break;
      case ONE: primaryCache = new OneCache<Ret,Args...>(key,pickle,unpickle); break;
      default: primaryCache = new RegCache<Ret,Args...>(key,pickle,unpickle);
    }
  }

  //Destructor
  template<typename T, typename Ret, typename ...Args>
  PersistentMemoized<T,Ret,Args...>::~PersistentMemoized(){
    this->cacheConsistent.lock();
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
  PersistentMemoized<T,Ret,Args...>::PersistentMemoized(MemCacheType type, 
    Cache<Ret,Args...>* secondaryCache){
    log("constructor2");
    this->primaryCache = NULL;
    this->secondaryCache = secondaryCache;
    this->setMemoryCache(type);
  }

  template<typename T, typename Ret, typename ...Args>
  void PersistentMemoized<T,Ret,Args...>::move(PersistentMemoized<T,Ret,Args...>&& rvalue){
    this->primaryCache = rvalue.primaryCache;
    this->secondaryCache = rvalue.secondaryCache;
    this->memCacheType = rvalue.memCacheType;
  }

  //Move Constructor
  template<typename T, typename Ret, typename ...Args>
  PersistentMemoized<T,Ret,Args...>::PersistentMemoized(PersistentMemoized<T,Ret,Args...>&& rvalue){
    log("move constructor");
    move(std::move(rvalue));
    rvalue.nullFields();
  }

  //Move Assignment
  template<typename T, typename Ret, typename ...Args>
  PersistentMemoized<T,Ret,Args...>& PersistentMemoized<T,Ret,Args...>::operator=(PersistentMemoized<T,Ret,Args...>&& rvalue){
    log("move assignment");
    if(this != &rvalue){
      deleteCaches();
      move(std::move(rvalue));
      rvalue.nullFields();
    }
    return *this;
  }

  template<typename T, typename Ret, typename ...Args>
  void PersistentMemoized<T,Ret,Args...>::copy(const PersistentMemoized<T,Ret,Args...>& lvalue){
    this->primaryCache = lvalue.primaryCache->clone();
    this->secondaryCache = NULL;
    if(lvalue.secondaryCache != NULL){
      this->secondaryCache = lvalue.secondaryCache->clone();
      this->memCacheType = lvalue.memCacheType;
    }
  }

  //Copy Constructor
  template<typename T, typename Ret, typename ...Args>
  PersistentMemoized<T,Ret,Args...>::PersistentMemoized(const PersistentMemoized<T,Ret,Args...>& lvalue)
  {
    log("copy constructor");
    copy(lvalue);
  }

  //Copy Assignment
  template<typename T, typename Ret, typename ...Args>
  PersistentMemoized<T,Ret,Args...>& PersistentMemoized<T,Ret,Args...>::operator=(const PersistentMemoized<T,Ret,Args...>& lvalue){
    log("copy assignment");
    if (this != &lvalue) { // no self-assignment
      deleteCaches();
      copy(lvalue);
    }
    return *this;
  }

  template<typename T, typename Ret, typename ...Args>
  void PersistentMemoized<T,Ret,Args...>::deleteCaches(){
    delete this->primaryCache;
    delete this->secondaryCache;
  }

  template<typename T, typename Ret, typename ...Args>
  Ret PersistentMemoized<T,Ret,Args...>::operator()(Args const&... args) {
    auto start = system_clock::now();
    Ret answer = solve(args...);
    auto end = system_clock::now();
    timeTaken = duration_cast<milliseconds>(end-start).count();
    return answer;
  }

  template<typename T, typename Ret, typename ...Args>
  void PersistentMemoized<T,Ret,Args...>::write(Args const&... args, Ret const& realAnswer){
    secondaryCache->put(args...,realAnswer);
    cacheConsistent.unlock();
  }

  template<typename T, typename Ret, typename ...Args>
  Ret PersistentMemoized<T,Ret,Args...>::solve(Args... args){
    std::optional<Ret> answer = primaryCache->get(args...);  
    if(answer){
      cacheHits++;
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
    cacheMisses++;
    Ret realAnswer = T::solve(args...);
    if(this->secondaryCache != NULL){
      this->cacheConsistent.lock();
      discard = std::async(&PersistentMemoized<T,Ret,Args...>::write,this,args..., realAnswer);
    }
    primaryCache->put(args..., realAnswer);
    return realAnswer;  
  }
}