#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <optional>
#include <unordered_map>

#include "../../interfaces/memcache.hpp"
#include "../../utils/log.hpp"
using std::string;
namespace cpst {
template <typename T, typename Ret, typename... Args>
class Memoized : public T {
 public:
  Memoized(MemCache<Ret, Args...>* newCache) : cache(newCache) {}
  Ret solve(Args... args) {
    if(!decision(args...)) return T::solve(args...);
    // Check if entry exists in primary cache
    std::optional<Ret> answer = cache->get(args...);
    if (answer) {
      // Entry exists in primary cache, return its value
      logOne("CACHE HIT");
      return answer.value();
    }
    // Getting to this point means that the entry does not exist in any cache
    // so the value for this entry must be calculated.
    Ret realAnswer = T::solve(args...);
    cache->put(args..., realAnswer);
    return realAnswer;
  }
  Ret operator()(Args... args) { return solve(args...); }

  /**
   * Destructor
   */
  ~Memoized() { delete cache; }

  /**
   * Copy constructor
   */
  Memoized(const Memoized& other)  
  {
    this->cache = other.cache->clone();
  }

  /**
   * Move constructor
   */
  Memoized(Memoized&& other) noexcept  
      : cache(std::exchange(other.cache, nullptr)) {}

  /**
   * Copy constructor
   */
  Memoized& operator=(const Memoized& other)  
  {
    if (this == &other) return *this;
    return *this = Memoized(other);
  }

  /**
   * Move assignment
   */
  Memoized& operator=(Memoized&& other) noexcept  
  {
    if (this == &other) return *this;
    std::swap(cache, other.cache);
    return *this;
  }

  /**
   * A method that returns a boolean indicating if this set of input arguments
   * should be memoized or not.
   */
  void setDecision(bool (*decision_)(Args...)){
    this->decision = decision_;
  }

  /**
   * Returns the underlying MemCache object.
   */ 
  MemCache<Ret, Args...>* getCache(){
    return this->cache;
  }

 private:
  bool (*decision)(Args...) = returnTrue<Args...>;
  MemCache<Ret, Args...>* cache;
};
}  // namespace cpst