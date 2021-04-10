#include <algorithm>
#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>

#include "../local.hpp"
#include "../utils/files.hpp"
#include "../utils/log.hpp"
#include "data-generation/generators.cpp"
#include "helpers/Timer.cpp"
#include "helpers/TrieNode.cpp"
#include "helpers/bigint.cpp"
typedef unsigned long long largestUnsigned;
using namespace std::chrono;
using namespace cpst;
using namespace std;

void addWordToTrie(std::string& word, TrieNode* head) {
  TrieNode* cur = head;
  for (char& c : word) {
    TrieNode* child = cur->getChild(c);
    if (child == nullptr) {
      TrieNode* temp = new TrieNode(cur->prefix + c);
      cur->setChild(c, temp);
      cur = temp;
    } else
      cur = child;
  }
  cur->isWord = true;
}
void completeTrie(TrieNode* head, string& dictPath) {
  std::ifstream infile(dictPath);
  std::string line;
  while (std::getline(infile, line)) {
    std::istringstream iss(line);
    std::string word;
    iss >> word;
    addWordToTrie(word, head);
  }
}

class TrieGen {
 public:
  virtual TrieNode* solve(string dictPath) = 0;
};

class TrieGenerator : public Memoizable<TrieNode*,string>, public TrieGen{
  public:
    TrieNode* solve(string dictPath){
      TrieNode* ret = new TrieNode(false);
      completeTrie(ret,dictPath);
      return ret;
    }
};

namespace fs = std::filesystem;
using namespace std::chrono_literals;
string genKey(string path){
  fs::path p = path;
  auto ftime = fs::last_write_time(p);
  std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime); 
  return path + std::asctime(std::localtime(&cftime));
}

void runTrie(TrieGen& generator, vector<string>& input,
              string outPath, bool cppersist) {
  vector<TrieNode*> toFree;
  Timer timer;
  timer.start();
  // Convert dictionary ("words.txt") into a Trie data structure twice.
  for(int i=0;i<2;i++){
    TrieNode* answer = generator.solve("words.txt");
    toFree.push_back(answer);
  }
  timer.end();
  for (vector<TrieNode*>::iterator it = toFree.begin(); it != toFree.end(); it++) {
    TrieNode::freeAll(*it);
  }
  appendRowToFile(outPath, timer.getRow());
}

void runTrie(vector<string>& input, string type,
              bool cppersist, bool recursive, bool keepCache) {
  string outPath = getOutPath("Trie5", type, cppersist, recursive, keepCache);
  if (recursive) {
    if (!cppersist) {
      TrieGenerator gen;
      runTrie(gen, input, outPath, cppersist);
    } 
    else {
      auto genMemo = getLocalMemoizedObj<TrieGenerator>(genKey, TrieNode::pickle, 
        TrieNode::unpickle, sha256);
      runTrie(genMemo, input, outPath, cppersist);
    }
  }
}

void runTrieWORep(vector<string>& input, bool cppersist,
                   bool recursive, bool keepCache, int seed) {
  shuffle(input.begin(), input.end(), default_random_engine(seed));
  runTrie(input, "WORep", cppersist, recursive, keepCache);
}

void runTrieWRep(vector<string>& input, bool cppersist,
                  bool recursive, bool keepCache, int seed) {
  srand(seed);
  vector<string> newInp;
  while (newInp.size() != input.size()) {
    newInp.push_back(input[rand() % input.size()]);
  }
  runTrie(newInp, "WRep", cppersist, recursive, keepCache);
}


std::vector<string> getFilePaths(){
  std::vector<string> ret;
  string dir = "./TrieGenInput";
  for(auto& file: fs::directory_iterator(dir)){
    fs::path path = file.path();
    std::string pathStr = path;
    ret.push_back(pathStr);
  }
  return ret;
}

int main(int argc, char const* argv[]) {
  int numInput = stoi(argv[1]);
  bool cppersist = stoi(argv[2]);
  bool recursive = stoi(argv[3]);
  bool keepCache = stoi(argv[4]);
  int seed = stoi(argv[5]);
  const char* version = argv[6];
  std::vector<string> filePaths = getFilePaths();
  if (std::strcmp(version, "worep") == 0) {
    runTrieWORep(filePaths, cppersist, recursive, keepCache, seed);
  }
  if (std::strcmp(version, "wrep") == 0) {
    runTrieWRep(filePaths, cppersist, recursive, keepCache, seed);
  }
  return 0;
}
