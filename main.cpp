#include <iostream>
#include "cache.h"
#include <string>
#include <optional>
using std::string;
int strtoi(string lol){
  return std::stoi(lol);
}
string intostr(int lol){
  return "hey";
}
int main(){
  Cache<int, int> cache(intostr, strtoi);
  std::optional<int> x = cache.get(1);
  std::cout << x.value_or(69);
}