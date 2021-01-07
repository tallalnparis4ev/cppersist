#include <fstream>
#include <sstream>
#include <string>
#include <optional>
#include "../../crypto/sha256.h"
using std::optional;
using std::nullopt;
using std::function;
using std::string;

template <typename Ret, typename ...Args> 
DiskCache<Ret,Args...>* DiskCache<Ret,Args...>::clone(){
  return new DiskCache<Ret,Args...>(this->key,this->pickle,this->unpickle,this->funcName);
}

template <typename Ret, typename ...Args> 
DiskCache<Ret,Args...>::DiskCache(string (*key)(const Args&...),
  string (*pickle)(const Ret&),Ret (*unpickle)(const string&), const string& funcName)
{
  this->key = key;
  this->pickle = pickle;
  this->unpickle = unpickle;
  this->funcName = funcName;
  this->outputPath = "./"+funcName+"/";
}

template <typename Ret, typename ...Args> 
string DiskCache<Ret,Args...>::makePathForKey(const string& key) {
  return this->outputPath + sha256(key) + ".txt";
}

template <typename Ret, typename ...Args> 
std::optional<Ret> DiskCache<Ret,Args...>::get(const Args&... args) {
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
void DiskCache<Ret,Args...>::put(const Args&... args, const Ret& value)  {
  std::ofstream myfile(makePathForKey(this->key(args...)));
  myfile << this->pickle(value);
  myfile.close();
}

template <typename Ret, typename ...Args> 
void DiskCache<Ret,Args...>::put(const string& key, const string& value){
  std::ofstream myfile(makePathForKey(key));
  myfile << value;
  myfile.close();
}