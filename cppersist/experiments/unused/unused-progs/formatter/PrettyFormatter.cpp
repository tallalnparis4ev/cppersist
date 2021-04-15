#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
namespace fs = std::filesystem;

class Time {
 public:
  Time() {}
  Time(bool isLast) { this->isLast = isLast; }
  void addTime(std::string& time) {
    total += std::stod(time);
    count++;
  }
  double getAverageInSeconds() const {
    double average = (total / count);
    return average / (Time::milli);
  }
  friend std::ostream& operator<<(std::ostream& out, const Time& time);
  bool isLast = false;
  std::string fileName;

 private:
  constexpr static double nano = 1000000000;
  constexpr static double milli = 1000;
  double total = 0;
  int count = 0;
};

std::ostream& operator<<(std::ostream& out, const Time& time) {
  if (time.isLast)
    out << std::to_string(time.getAverageInSeconds()) << std::endl;
  else
    out << std::to_string(time.getAverageInSeconds()) << ",";
  return out;
}

class PrettyFormatter {
 public:
  void formatAllDirs(std::string& dir) {
    for (auto& file : fs::directory_iterator(dir)) {
      fs::path path = file.path();
      std::string pathStr = path;
      std::string name = path.filename();
      if (name == "Caches") continue;
      formatAllFiles(pathStr);
    }
  }
  void formatAllFiles(std::string& dir) {
    std::vector<std::string> files;
    for (auto& file : fs::directory_iterator(dir)) {
      fs::path path = file.path();
      std::string pathStr = path;
      std::string name = path.filename();
      if (name == "Iter" || name == "Rec") {
        formatAllFiles(pathStr);
        continue;
      }
      if (name == "googleSheet.gsheet") {
        fs::remove(path);
        continue;
      }
      if (name == "googleSheet.out") continue;
      files.push_back(name);
    }
    if (files.size() == 0) return;
    std::vector<Time> realTimes;
    std::vector<std::string> output;
    std::sort(files.begin(), files.end());
    for (auto& name : files) {
      std::string path = dir + "/" + name;
      formatSingleFile(name, path, realTimes, output);
    }

    int i = realTimes.size() == 6 ? 1 : 0;
    int vectorInd = 0;
    std::string names[3] = {"Seq", "WORep", "WRep"};
    std::ofstream outFile;
    outFile.open(dir + "/googleSheet.gsheet", std::ios_base::app);
    for (; i < 3; i++) {
      outFile << names[i] << std::endl
              << "Method, Average Time Taken (10 runs) (s)" << std::endl
              << "With Cppersist (persistent cache kept between runs), "
              << realTimes[vectorInd] << "With Cppersist, "
              << realTimes[vectorInd + 1] << "Without Cppersist, "
              << realTimes[vectorInd + 2] << std::endl;
      vectorInd += 3;
    }
    outFile << std::endl;
    for (const std::string& out : output) {
      outFile << out;
      outFile << std::endl;
    }
  }

  void formatSingleFile(std::string& name, std::string& file,
                        std::vector<Time>& realTimes,
                        std::vector<std::string>& output) {
    std::ifstream curFile(file);
    std::string line;
    Time times[3] = {Time(), Time(), Time(true)};
    while (std::getline(curFile, line)) {
      std::string delim[3];
      std::stringstream stream(line);
      int ind = 0;
      while (stream.good()) {
        std::string col;
        getline(stream, col, ',');
        times[ind++].addTime(col);
      }
    }
    realTimes.push_back(times[2]);
    std::stringstream outStr;
    outStr << name << std::endl
           << "System (s),User (s),Real (s)" << std::endl
           << times[0] << times[1] << times[2];
    output.push_back(outStr.str());
  }
};

int main(int argc, char const* argv[]) {
  PrettyFormatter pretty;
  std::string dir = argv[1];
  pretty.formatAllDirs(dir);
  return 0;
}