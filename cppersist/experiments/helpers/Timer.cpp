#include <sys/times.h>

#include <chrono>
typedef unsigned long long chronoTime;

class Timer {
 public:
  void start() {
    times(&startCpu);
    startReal = std::chrono::high_resolution_clock::now();
  }
  void end() {
    times(&endCpu);
    std::chrono::time_point<std::chrono::high_resolution_clock> nowReal =
        std::chrono::high_resolution_clock::now();

    realTime += std::chrono::duration_cast<std::chrono::nanoseconds>(
                           nowReal - startReal)
                           .count();

    sysCpu += endCpu.tms_stime - startCpu.tms_stime;
    userCpu += endCpu.tms_utime - startCpu.tms_utime;
  }

  std::string headers() { return "System (ms), User (ms), Real (ms)"; }

  std::string getRow() {
    return std::to_string(sysCpu) + "," + std::to_string(userCpu) + "," +
           std::to_string(realTime);
  }

 private:
  clock_t sysCpu = 0;
  clock_t userCpu = 0;
  chronoTime realTime = 0;
  // string realTimeStr;

  std::chrono::time_point<std::chrono::high_resolution_clock> startReal;
  struct tms startCpu;
  struct tms endCpu;
};