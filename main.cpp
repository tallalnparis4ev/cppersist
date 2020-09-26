#include <iostream>
#include "cache.h"

int main(){
  std::cout << "Hello World!\n";
  Cache<int, int> cache;
  int x = cache.get(1);
  std::cout << x;
}