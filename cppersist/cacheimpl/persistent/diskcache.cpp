#include <fstream>
#include <sstream>
#include <string>
#include <optional>
#include <filesystem>
#include "../../crypto/sha256.h"
using std::optional;
using std::nullopt;
using std::function;
using std::string;
namespace cpst{
  template <typename Ret, typename ...Args> 
  const string DiskCache<Ret,Args...>::OUT_DIR = "persist";
  
  template <typename Ret, typename ...Args> 
  DiskCache<Ret,Args...>* DiskCache<Ret,Args...>::clone(){
    return new DiskCache<Ret,Args...>(this->key,this->pickle,this->unpickle,this->funcName);
  }

  template <typename Ret, typename ...Args> 
  DiskCache<Ret,Args...>::DiskCache(string (*key)(Args...),
    string (*pickle)(Ret),Ret (*unpickle)(string), string funcName)
  {
    this->key = key;
    this->pickle = pickle;
    this->unpickle = unpickle;
    this->funcName = funcName;
    std::filesystem::create_directories(OUT_DIR+"/"+funcName);
    this->outputPath = "./"+OUT_DIR+"/"+funcName+"/";
  }

  template <typename Ret, typename ...Args> 
  string DiskCache<Ret,Args...>::makePathForKey(const string& key) {
    return this->outputPath + sha256(key) + ".txt";
  }

  template <typename Ret, typename ...Args> 
  std::optional<Ret> DiskCache<Ret,Args...>::get(const Args&... args) {
    std::ifstream inFile(makePathForKey(this->key(args...)));
    if (!inFile) {
        logOne("Unable to open file");
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
}