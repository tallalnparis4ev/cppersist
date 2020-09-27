#include <iostream>
#include "cache.h"
#include <string>
#include <optional>
using std::string;
int strtoi(string lol){
  return std::stoi(lol);
}
string intostr(int lol){
  return std::to_string(lol);
}

string keymaker(int lol){
  return std::to_string(lol);
}
int main(){
  Cache<int, int> cache(keymaker,intostr, strtoi);
  cache.put(1,1);
  std::optional<int> x = cache.get(1);
  cache.put(2,2);
  std::optional<int> y = cache.get(2);
  std::cout << x.value_or(69);
  std::cout << y.value_or(69);
}