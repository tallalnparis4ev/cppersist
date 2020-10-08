#include "persister.hpp"
#include <iostream>
class Solver: public PersistentMemoizable<int, int, int, int>{
  public:
    int solve(int x, int y, int z) override {
      std::cout << "Original" << std::endl;
      if(x<0) return 42;
      return solve(x-1,1,1);
    } 
};

int main(){
  Solver solver;
  PersistentMemoized memoizedSolver2 = Persister::getMemoizedObj<Solver>(solver);
  memoizedSolver2.solve(4,2,3);
}