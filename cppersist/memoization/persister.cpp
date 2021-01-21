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

  //Helper function to null fields 
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
      //If there's no primary cache or secondary cache, then this method
      //should do nothing.
      if(this->primaryCache == NULL) return;
      
      //Make the primary cache the secondary cache
      this->secondaryCache = this->primaryCache;
    }
    //The primary cache is being updated, therefore delete the old one
    else if(this->primaryCache != NULL) delete this->primaryCache; 

    //Get the key, pickle and unpickle functions from the initial cache
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

  //A helper function that moves fields from another PersistentMemoized object
  template<typename T, typename Ret, typename ...Args>
  void PersistentMemoized<T,Ret,Args...>::move(PersistentMemoized<T,Ret,Args...>&& rvalue){
    this->primaryCache = rvalue.primaryCache;
    this->secondaryCache = rvalue.secondaryCache;
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
    if(this != &rvalue){ // no self-assignment
      deleteCaches();
      move(std::move(rvalue));
      rvalue.nullFields();
    }
    return *this;
  }

  //A helper function that copies from another PersistentMemoized object
  template<typename T, typename Ret, typename ...Args>
  void PersistentMemoized<T,Ret,Args...>::copy(const PersistentMemoized<T,Ret,Args...>& lvalue){
    this->primaryCache = lvalue.primaryCache->clone();
    this->secondaryCache = NULL;
    if(lvalue.secondaryCache != NULL){
      this->secondaryCache = lvalue.secondaryCache->clone();
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

  //Deletes pointers
  template<typename T, typename Ret, typename ...Args>
  void PersistentMemoized<T,Ret,Args...>::deleteCaches(){
    delete this->primaryCache;
    delete this->secondaryCache;
  }

  template<typename T, typename Ret, typename ...Args>
  Ret PersistentMemoized<T,Ret,Args...>::operator()(Args const&... args) {
    auto start = high_resolution_clock::now();
    Ret answer = solve(args...);
    auto end = high_resolution_clock::now();
    solveTime = duration_cast<nanoseconds>(end-start).count();
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
    auto start = high_resolution_clock::now();
    auto end = high_resolution_clock::now();
    //Check if entry exists in primary cache
    if(answer){
      //Entry exists in primary cache, return its value
      end = high_resolution_clock::now();
      hitTime = duration_cast<nanoseconds>(end-start).count();
      cacheHits++;
      miss = false;
      logOne("CACHE HIT");
      return answer.value();
    }
    if(this->secondaryCache != NULL){
      //Check if entry exists in secondary cache
      answer = secondaryCache->get(args...);
      if(answer){
        logOne("SECONDARY CACHE HIT");
        return answer.value();
      }
    }

    end = high_resolution_clock::now();
    missTime = duration_cast<nanoseconds>(end-start).count();
    cacheMisses++;
    miss = true;

    //Getting to this point means that the entry does not exist in any cache
    //so the value for this entry must be calculated.
    Ret realAnswer = T::solve(args...);
    
    if(this->secondaryCache != NULL){
      //This lock ensures that the persistent cache will have the same contents
      //regardless of using an in-memory cache or not.
      this->cacheConsistent.lock();
      //Write this new entry to secondary cache in a seprate thread
      discard = std::async(&PersistentMemoized<T,Ret,Args...>::write,this,args..., realAnswer);
    }

    start = high_resolution_clock::now(); 
    primaryCache->put(args..., realAnswer);
    end = high_resolution_clock::now();
    missPenalty = duration_cast<nanoseconds>(end-start).count();
    return realAnswer;  
  }

  template<typename T, typename Ret, typename ...Args>
  void PersistentMemoized<T,Ret,Args...>::resetTimes(){
    hitTime = 0;
    missTime = 0;
    missPenalty = 0;
  } 
}