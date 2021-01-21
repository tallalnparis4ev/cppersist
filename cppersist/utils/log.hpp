#ifndef LOG_H_FILE
#define LOG_H_FILE

#include <iostream>
using namespace std;
namespace cpst{
  template <typename T>
  void print(T value) {
    cout << value << ", ";
  }

  template <typename First, typename... Rest>
  void print(First firstValue, Rest... rest) {
    print(firstValue);
    print(rest...);
  }

  template <typename First, typename... Rest>
  void log(First firstValue, Rest... rest) {
    print(firstValue,rest...);
    cout << endl;
  }

  template <typename Single>
  void logOne(Single singleValue){
    cout << singleValue << endl;
  }
}
#endif
