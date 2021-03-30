#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <optional>
#include <unordered_map>

#include "../../cacheimpl/memory/lrucache.hpp"
#include "../../cacheimpl/memory/onecache.hpp"
#include "../../cacheimpl/memory/regcache.hpp"
#include "../../utils/log.hpp"
using std::string;
namespace cpst {
// Helper function to null fields
template <typename T, typename Ret, typename... Args>
void PersistentMemoized<T, Ret, Args...>::nullFields() {
  this->primaryCache = NULL;
  this->secondaryCache = NULL;
}

template <typename T, typename Ret, typename... Args>
void PersistentMemoized<T, Ret, Args...>::setMemoryCache(MemCacheType type) {
  setMemoryCache(type, 1000);
}

template <typename T, typename Ret, typename... Args>
void PersistentMemoized<T, Ret, Args...>::setMemoryCache(MemCacheType type,
                                                         int size) {
  if (this->secondaryCache == NULL) {
    // If there's no primary cache or secondary cache, then this method
    // should do nothing.
    if (this->primaryCache == NULL) return;

    // Make the primary cache the secondary cache
    this->secondaryCache = this->primaryCache;
  }
  // The primary cache is being updated, therefore delete the old one
  else if (this->primaryCache != NULL)
    delete this->primaryCache;

  // Get the key, pickle and unpickle functions from the initial cache
  auto key = this->secondaryCache->getKey();
  auto pickle = this->secondaryCache->getPickle();
  auto unpickle = this->secondaryCache->getUnpickle();

  switch (type) {
    case MemCacheType::LRU:
      primaryCache = new LRUCache<Ret, Args...>(size, key, pickle, unpickle);
      break;
    case MemCacheType::REGULAR:
      primaryCache = new RegCache<Ret, Args...>(size, key, pickle, unpickle);
      break;
    case MemCacheType::ONE:
      primaryCache = new OneCache<Ret, Args...>(key, pickle, unpickle);
      break;
    default:
      primaryCache = new RegCache<Ret, Args...>(key, pickle, unpickle);
  }
}

// Destructor
template <typename T, typename Ret, typename... Args>
PersistentMemoized<T, Ret, Args...>::~PersistentMemoized() {
  this->cacheConsistent.lock();
  deleteCaches();
}

// Constructor1
template <typename T, typename Ret, typename... Args>
PersistentMemoized<T, Ret, Args...>::PersistentMemoized(
    Cache<Ret, Args...>* cache) {
  log("constructor1");
  this->primaryCache = cache;
  this->secondaryCache = NULL;
}

// Constructor2
template <typename T, typename Ret, typename... Args>
PersistentMemoized<T, Ret, Args...>::PersistentMemoized(
    MemCacheType type, Cache<Ret, Args...>* secondaryCache) {
  log("constructor2");
  this->primaryCache = NULL;
  this->secondaryCache = secondaryCache;
  this->setMemoryCache(type);
}

// A helper function that moves fields from another PersistentMemoized object
template <typename T, typename Ret, typename... Args>
void PersistentMemoized<T, Ret, Args...>::move(
    PersistentMemoized<T, Ret, Args...>&& rvalue) {
  this->primaryCache = rvalue.primaryCache;
  this->secondaryCache = rvalue.secondaryCache;
}

// Move Constructor
template <typename T, typename Ret, typename... Args>
PersistentMemoized<T, Ret, Args...>::PersistentMemoized(
    PersistentMemoized<T, Ret, Args...>&& rvalue) {
  log("move constructor");
  move(std::move(rvalue));
  rvalue.nullFields();
}

// Move Assignment
template <typename T, typename Ret, typename... Args>
PersistentMemoized<T, Ret, Args...>&
PersistentMemoized<T, Ret, Args...>::operator=(
    PersistentMemoized<T, Ret, Args...>&& rvalue) {
  log("move assignment");
  if (this != &rvalue) {  // no self-assignment
    deleteCaches();
    move(std::move(rvalue));
    rvalue.nullFields();
  }
  return *this;
}

// A helper function that copies from another PersistentMemoized object
template <typename T, typename Ret, typename... Args>
void PersistentMemoized<T, Ret, Args...>::copy(
    const PersistentMemoized<T, Ret, Args...>& lvalue) {
  this->primaryCache = lvalue.primaryCache->clone();
  this->secondaryCache = NULL;
  if (lvalue.secondaryCache != NULL) {
    this->secondaryCache = lvalue.secondaryCache->clone();
  }
}

// Copy Constructor
template <typename T, typename Ret, typename... Args>
PersistentMemoized<T, Ret, Args...>::PersistentMemoized(
    const PersistentMemoized<T, Ret, Args...>& lvalue) {
  log("copy constructor");
  copy(lvalue);
}

// Copy Assignment
template <typename T, typename Ret, typename... Args>
PersistentMemoized<T, Ret, Args...>&
PersistentMemoized<T, Ret, Args...>::operator=(
    const PersistentMemoized<T, Ret, Args...>& lvalue) {
  log("copy assignment");
  if (this != &lvalue) {  // no self-assignment
    deleteCaches();
    copy(lvalue);
  }
  return *this;
}

// Deletes pointers
template <typename T, typename Ret, typename... Args>
void PersistentMemoized<T, Ret, Args...>::deleteCaches() {
  delete this->primaryCache;
  delete this->secondaryCache;
}

template <typename T, typename Ret, typename... Args>
Ret PersistentMemoized<T, Ret, Args...>::operator()(Args const&... args) {
  return solve(args...);
}

template <typename T, typename Ret, typename... Args>
void PersistentMemoized<T, Ret, Args...>::write(Args const&... args,
                                                Ret const& realAnswer) {
  secondaryCache->put(args..., realAnswer);
  cacheConsistent.unlock();
}

template <typename T, typename Ret, typename... Args>
Ret PersistentMemoized<T, Ret, Args...>::solve(Args... args) {
  // Check if entry exists in primary cache
  std::optional<Ret> answer = primaryCache->get(args...);
  if (answer) {
    // Entry exists in primary cache, return its value
    logOne("CACHE HIT");
    return answer.value();
  }
  if (this->secondaryCache != NULL) {
    // Check if entry exists in secondary cache
    answer = secondaryCache->get(args...);
    if (answer) {
      logOne("SECONDARY CACHE HIT");
      return answer.value();
    }
  }
  // Getting to this point means that the entry does not exist in any cache
  // so the value for this entry must be calculated.
  Ret realAnswer = T::solve(args...);

  if (this->secondaryCache != NULL) {
    // This lock ensures that the persistent cache will have the same contents
    // regardless of using an in-memory cache or not.
    this->cacheConsistent.lock();
    // Write this new entry to secondary cache in a seprate thread
    discard = std::async(&PersistentMemoized<T, Ret, Args...>::write, this,
                         args..., realAnswer);
  }
  primaryCache->put(args..., realAnswer);
  return realAnswer;
}
}  // namespace cpst