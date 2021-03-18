#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
namespace fs = std::filesystem;

class Time{
  public:
    Time(){}
    Time(bool isLast){this->isLast = isLast;}
    void addTime(std::string& time){
      total += std::stod(time);
      count++;
    }
    double getAverageInSeconds() const {
      double average = (total/count);
      return average/(Time::milli);
    }
    friend std::ostream& operator<< (std::ostream& out, const Time& time);
    bool isLast = false;
  private:
    constexpr static double nano = 1000000000;
    constexpr static double milli = 1000;
    double total = 0;
    int count = 0;
};

std::ostream& operator<< (std::ostream& out, const Time& time)
{
  if(time.isLast)
    out << std::to_string(time.getAverageInSeconds()) << std::endl;
  else   
    out << std::to_string(time.getAverageInSeconds()) << ",";
  return out; 
}

class PrettyFormatter{
  public:
    void formatAllFiles(std::string& dir){
      for(auto& file: fs::directory_iterator(dir)){
        fs::path path = file.path();
        std::string pathStr = path;
        std::string name = path.filename();
        formatSingleFile(pathStr, name);
      }
    }
    void formatSingleFile(std::string& file, std::string& name){
      std::ifstream curFile(file);
      std::string line;
      Time times[3] = {Time(), Time(), Time(true)};
      while (std::getline(curFile, line))
      {
        std::string delim[3];
        std::stringstream stream(line); 
        int ind = 0;
        while(stream.good()) {
            std::string col;
            getline(stream, col, ',');
            times[ind++].addTime(col);
        }
      }
      std::cout << name << std::endl;
      std::ofstream outFile;
      outFile.open(file, std::ios_base::app);
      outFile << std::endl 
              << std::endl 
              << name
              << std:: endl
              << "System (s),User (s),Real (s)" 
              << std::endl
              << times[0] << times[1] << times[2];
    }
    
};
 

int main(int argc, char const *argv[])
{
  PrettyFormatter pretty;
  std::string dir = argv[1];
  pretty.formatAllFiles(dir);
  return 0;
}