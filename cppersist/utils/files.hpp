#include <fstream>
using namespace std;
void appendRowToFile(const string& fileName, const string& row){
  std::ofstream dataFile(fileName, std::ios_base::app);
  dataFile << row << endl;
}