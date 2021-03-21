#ifndef FILES_H_FILE
#define FILES_H_FILE

#include <fstream>
#include <filesystem>
using namespace std;
void appendRowToFile(const string& fileName, const string& row) {
  std::ofstream dataFile(fileName, std::ios_base::app);
  dataFile << row << endl;
}

string getOutPath(string algo, string type, bool cppersist, bool recursive,
                  bool keepCache) {
  string delCache = !keepCache ? "DelCache" : "";
  string rec = (recursive ? "Rec" : "Iter");
  string affix = rec + (cppersist ? "Cpst" : "NoCpst") + delCache;
  std::filesystem::create_directories("./data/" + algo);
  return "./data/" + algo + "/" + type + affix + ".csv";
}

#endif