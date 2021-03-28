#ifndef INT_SIMPLE_H
#define INT_SIMPLE_H
#include <string>
using std::string;
class intSimple {
 public:
  static string pickle(int value) { return std::to_string(value); }
  static int unpickle(string value) { return std::stoi(value); }
};
#endif
