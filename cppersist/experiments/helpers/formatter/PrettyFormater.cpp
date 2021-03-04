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
      total += std::stoull(time);
      count++;
    }
    unsigned long long getAverageInSeconds() const {
      return (total/count)/(Time::nano);
    }
    friend std::ostream& operator<< (std::ostream& out, const Time& time);
    bool isLast = false;
  private:
    const static unsigned long long nano = 1000000000;
    unsigned long long total = 0;
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

class PrettyFormater{
  public:
    void formatAllFiles(std::string& dir){
      for(auto& file: fs::directory_iterator(dir)){
        std::string path = file.path();
        formatSingleFile(path);
      }
    }
    void formatSingleFile(std::string& file){
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
      std::ofstream outFile;
      outFile.open(file, std::ios_base::app);
      outFile << std::endl 
              << std::endl 
              << "System (ms),User (ms),Real (ms)" 
              << std::endl << times[0] << times[1] 
              << times[2];
    }
    
};
 

int main(int argc, char const *argv[])
{
  PrettyFormater pretty;
  std::string dir = argv[1];
  pretty.formatAllFiles(dir);
  return 0;
}