#pragma once
#include <vector>
using std::string;
class ArgGenerator {
 public:
  ArgGenerator(int seed) { srand(seed); };
  ArgGenerator() { srand(time(NULL)); };
  virtual string getNext() = 0;
  bool hasNext();
};

class StringGenerator : public ArgGenerator {
 public:
  using ArgGenerator::ArgGenerator;
  const char lowercase[27] = "abcdefghijklmnopqrstuvwxyz";
  int max = 2;
  string getNext() {
    return string() + lowercase[rand() % 26] + lowercase[rand() % 26] +
           lowercase[rand() % 26];
  };
};

class IntGenerator : public ArgGenerator {
 public:
  using ArgGenerator::ArgGenerator;
  IntGenerator(int seed, int min, int max) {
    srand(seed);
    this->min = min;
    this->max = max;
  }
  int min;
  int max;
  string getNext() { return to_string((rand() % max) + min); };
};
