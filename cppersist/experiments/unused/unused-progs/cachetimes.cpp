#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "../local.hpp"
// #include "../mongo.hpp"
#include "../utils/log.hpp"
#include "data-generation/generators.cpp"
using namespace cpst;
using namespace std;

class ToyExample : public Memoizable<string, string> {
 public:
  string solve(string arg) override { return arg; };
};

void getDiskMetrics(int seed) {
  std::ofstream dataFile("./data/diskCacheTimes.out", std::ios_base::app);
  size_t totalHitTime = 0;
  size_t totalMissTime = 0;
  size_t totalMissPenalty = 0;
  StringGenerator sg(seed);
  int numberOfRuns = 30000;
  PersistentMemoized toyExample = getLocalMemoizedObj<ToyExample>(
      identity<string>, identity<string>, identity<string>, "test",
      identity<string>);
  for (int i = 0; i < numberOfRuns; i++) {
    string input = sg.getNext();
    toyExample(input);
    if (toyExample.miss) {
      totalMissTime += toyExample.missTime;
      totalMissPenalty += toyExample.missPenalty;
    } else
      totalHitTime += toyExample.hitTime;
    toyExample.resetTimes();
  }
  dataFile << "Hit Time, Miss Time, Miss Penalty" << endl;
  dataFile << (totalHitTime / toyExample.cacheHits) << ", "
           << (totalMissTime / toyExample.cacheMisses) << ", "
           << (totalMissPenalty / toyExample.cacheMisses) << endl;
  dataFile << "With " << toyExample.cacheHits << " cache hits and "
           << toyExample.cacheMisses << " cache misses." << endl;
  dataFile.close();
}

// void getMongoMetrics(int seed) {
//   std::ofstream dataFile("./data/mongoCacheTimes.out", std::ios_base::app);
//   size_t totalHitTime = 0;
//   size_t totalMissTime = 0;
//   size_t totalMissPenalty = 0;
//   StringGenerator sg(seed);
//   int numberOfRuns = 30000;
//   PersistentMemoized toyExample = getMongoMemoizedObj<ToyExample>(
//       identity<string>, identity<string>, identity<string>, "localhost:5000",
//       "test", identity<string>);
//   for (int i = 0; i < numberOfRuns; i++) {
//     string input = sg.getNext();
//     toyExample(input);
//     if (toyExample.miss) {
//       totalMissTime += toyExample.missTime;
//       totalMissPenalty += toyExample.missPenalty;
//     } else
//       totalHitTime += toyExample.hitTime;
//     toyExample.resetTimes();
//   }
//   dataFile << "Hit Time, Miss Time, Miss Penalty" << endl;
//   dataFile << (totalHitTime / toyExample.cacheHits) << ", "
//            << (totalMissTime / toyExample.cacheMisses) << ", "
//            << (totalMissPenalty / toyExample.cacheMisses) << endl;
//   dataFile << "With " << toyExample.cacheHits << " cache hits and "
//            << toyExample.cacheMisses << " cache misses." << endl;
//   dataFile.close();
// }

int main(int argc, char const *argv[]) {
  // StringGenerator sg(4);
  // int numberOfRuns = 30000;
  // vector<string> first;
  // for (size_t i = 0; i < numberOfRuns; i++)
  // {
  //   string next = sg.getNext();
  //   first.push_back(next);
  // }
  // StringGenerator sg2(4);

  // for (size_t i = 0; i < numberOfRuns; i++)
  // {
  //   string snd = sg2.getNext();
  //   if(first[i] != snd){
  //     cout << first[i] << endl;
  //     cout << snd << endl;
  //     cout << "not equal" << endl;
  //     break;
  //   }
  // }
  getDiskMetrics(1);
  // getMongoMetrics();

  return 0;
}
