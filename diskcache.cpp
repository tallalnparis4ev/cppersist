#include <fstream>
#include <sstream>
#include <string>
#include <optional>
using std::optional;
using std::nullopt;
using std::function;
using std::string;

template <typename Ret, typename ...Args> 
string DiskCache<Ret,Args...>::makePathForKey(string key) {
  return this->outputPath + key + ".txt";
}

template <typename Ret, typename ...Args> 
std::optional<Ret> DiskCache<Ret,Args...>::get(Args... args) {
  std::ifstream inFile(makePathForKey(this->key(args...)));
  if (!inFile) {
      std::cout << "Unable to open file" << std::endl;
      return nullopt;
  }    
  std::ostringstream ss;
  ss << inFile.rdbuf(); 
  string str = ss.str();
  inFile.close();
  return optional<Ret>{this->unpickle(str)};
}

template <typename Ret, typename ...Args> 
void DiskCache<Ret,Args...>::put(Args... args, Ret value)  {
  std::cout << "keying for put: " << this->key(args...) << std::endl;
  std::ofstream myfile(makePathForKey(this->key(args...)));
  myfile << this->pickle(value);
  myfile.close();
}