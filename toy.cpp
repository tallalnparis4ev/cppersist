#include "persister.hpp"
#include <iostream>
class Lol2: public Persister<int, int, int, int>{
  public:
    int solve(int x, int y, int z) override {
      std::cout << "Original" << std::endl;
      if(x<0) return 69;
      return solve(x-1,1,1);
    } 
};

class Lol3: public Lol2{
  public:
    int solve(int x, int y, int z) override {
      std::cout << "Modified" << std::endl;
      return Lol2::solve(x,1,1);
    } 
};

int main(){
  // Lol lol;
  // lol.getAnswer(4);
  Lol3 Lol3;
  Lol3.solve(4,1,1);
}