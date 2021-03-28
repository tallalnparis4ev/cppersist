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
  template<typename T, typename Ret, typename... Args>
  class Memoized : public T{
    public:
      Memoized(MemCache<Ret, Args...>* newCache): cache(newCache) {}
      Ret solve(Args... args){
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
      Ret operator()(Args... args){
        return solve(args...);
      }

      ~Memoized()
      {
          delete cache;
      }

      Memoized(const Memoized& other) // copy constructor
      {
        this->cache = other.cache->clone();
      }

      Memoized(Memoized&& other) noexcept // move constructor
      : cache(std::exchange(other.cache, nullptr))
      {
      }

      Memoized& operator=(const Memoized& other) // copy assignment
      {
          if (this == &other) return *this;
          return *this = Memoized(other);
      }

      Memoized& operator=(Memoized&& other) noexcept // move assignment
      {
          if (this == &other) return *this;
          std::swap(cache, other.cache);
          return *this;
      } 
    private:
      MemCache<Ret, Args...>* cache;
  };
}  // namespace cpst