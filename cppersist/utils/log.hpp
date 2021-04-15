#ifndef LOG_H_FILE
#define LOG_H_FILE
// A simple logging file
#include <iostream>
using namespace std;
namespace cpst {
bool shouldLog = false;  // if true log statements will be output to stdout,
                         // otherwise they won't.
template <typename T>
void print(T value) {
  if (!shouldLog) return;
  cout << value << ", ";
}

template <typename First, typename... Rest>
void print(First firstValue, Rest... rest) {
  if (!shouldLog) return;
  print(firstValue);
  print(rest...);
}

template <typename First, typename... Rest>
void log(First firstValue, Rest... rest) {
  if (!shouldLog) return;
  print(firstValue, rest...);
  cout << endl;
}

template <typename Single>
void logOne(Single singleValue) {
  if (!shouldLog) return;
  cout << singleValue << endl;
}
}  // namespace cpst
#endif
