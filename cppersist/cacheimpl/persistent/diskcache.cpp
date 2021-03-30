#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>

#include "../../crypto/sha256.h"
using std::nullopt;
using std::optional;
using std::string;
namespace cpst {
template <typename Ret, typename... Args>
const string DiskCache<Ret, Args...>::OUT_DIR = "persist";

template <typename Ret, typename... Args>
DiskCache<Ret, Args...>* DiskCache<Ret, Args...>::clone() {
  return new DiskCache<Ret, Args...>(this->key, this->pickle, this->unpickle,
                                     this->hash, this->funcName);
}

template <typename Ret, typename... Args>
DiskCache<Ret, Args...>::DiskCache(string (*key)(Args...),
                                   string (*pickle)(Ret),
                                   Ret (*unpickle)(string),
                                   string (*hash)(string), string funcName)
    : DiskCache(key, pickle, unpickle, hash) {
  this->funcName = funcName;
  std::filesystem::create_directories(OUT_DIR + "/" + funcName);
  this->outputPath = "./" + OUT_DIR + "/" + funcName + "/";
}

template <typename Ret, typename... Args>
string DiskCache<Ret, Args...>::makePathForKey(const string& key) {
  return this->outputPath + this->hash(key) + ".out";
}

template <typename Ret, typename... Args>
std::optional<Ret> DiskCache<Ret, Args...>::get(const Args&... args) {
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

template <typename Ret, typename... Args>
void DiskCache<Ret, Args...>::put(const Args&... args, const Ret& value) {
  std::ofstream myfile(makePathForKey(this->key(args...)));
  myfile << this->pickle(value);
  myfile.close();
}
}  // namespace cpst