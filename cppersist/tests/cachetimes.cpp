#include "../local.hpp"
#include "../mongo.hpp"
#include "../utils/log.hpp"
#include "data-generation/generators.cpp"
#include <fstream>
#include <sstream>
#include <vector>
using namespace cpst;
using namespace std;

class ToyExample : public PersistentMemoizable<string, string>{
  public:
    string solve(string arg) override {
      return arg;
    };
};

string key(string result){
  return result;
}

void getMetrics(){
  std::ofstream dataFile("./dataFile.txt", std::ios_base::app);
  size_t totalHitTime = 0;
  size_t totalMissTime = 0;
  size_t totalMissPenalty = 0;
  StringGenerator sg(4);
  int numberOfRuns = 30000;
  PersistentMemoized toyExample = getLocalMemoizedObj<ToyExample>(key,key,key,"test");
  for(int i=0;i<numberOfRuns;i++){
    string input = sg.getNext();
    toyExample(input);
    if(toyExample.miss){
      totalMissTime += toyExample.missTime;
      totalMissPenalty += toyExample.missPenalty;
    }
    else totalHitTime += toyExample.hitTime;
  }
  dataFile << "Hit Time, Miss Time, Miss Penalty" << endl;
  dataFile << (totalHitTime/toyExample.cacheHits) << ", "
  << (totalMissTime/toyExample.cacheMisses) << ", "
  << (totalMissPenalty/toyExample.cacheMisses)
  << endl;
  dataFile << "With " << toyExample.cacheHits << " cache hits and "
  << toyExample.cacheMisses << " cache misses." << endl;
  dataFile.close();
}

int main(int argc, char const *argv[])
{
  getMetrics();
  return 0;
}
