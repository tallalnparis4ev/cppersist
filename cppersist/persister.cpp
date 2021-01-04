#include <optional>
#include <functional>
#include <iostream>
using std::function;
using std::string;

template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...>::PersistentMemoized(Cache<Ret,Args...>* cache){
  this->persistentCache = cache;
}

template<typename T, typename Ret, typename ...Args>
PersistentMemoized<T,Ret,Args...>::~PersistentMemoized(){
  free(this->persistentCache);
}

template<typename T, typename Ret, typename ...Args>
Ret PersistentMemoized<T,Ret,Args...>::operator()(Args... args) {
  return solve(args...);
}

template<typename T, typename Ret, typename ...Args>
Ret PersistentMemoized<T,Ret,Args...>::solve(Args... args){
  std::optional<Ret> answer = persistentCache->get(args...);  
  if(answer){
    std::cout << "CACHE HIT" << std::endl;
    return answer.value();
  }
  Ret realAnswer = T::solve(args...);
  persistentCache->put(args..., realAnswer);
  return realAnswer;  
}
