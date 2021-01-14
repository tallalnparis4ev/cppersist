#ifndef CACHE_H_FILE
#define CACHE_H_FILE

#include <optional>
#include <string>
namespace cpst{
  template <typename Ret, typename ...Args> 
  class Cache
  {
    public:
      virtual std::optional<Ret> get(const Args&...) = 0;
      virtual void put(const Args&..., const Ret&) = 0;
      virtual Cache* clone() = 0;
      virtual ~Cache() {};
      auto getKey(){return key;}
      auto getPickle(){return pickle;}
      auto getUnpickle(){return unpickle;}
    protected:
      std::string (*key)(Args...);
      std::string (*pickle)(Ret);
      Ret (*unpickle)(std::string);
  };
}
#endif