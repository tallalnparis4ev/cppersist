#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "../local.hpp"
#include "../utils/files.hpp"
#include "../utils/log.hpp"
#include "data-generation/generators.cpp"
#include "helpers/Timer.cpp"
#include "helpers/TrieNode.cpp"
typedef unsigned long long largestUnsigned;
using namespace std::chrono;
using namespace cpst;
using namespace std;

// Used for Trie construction, adds 'word' from dictionary to 'head'
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

// Construct a trie, with root = 'head', from the words in the dictionary at
// 'path'
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

class TrieGenerator : public Memoizable<TrieNode*, string>, public TrieGen {
 public:
  TrieNode* solve(string dictPath) {
    TrieNode* ret = new TrieNode(false);
    completeTrie(ret, dictPath);
    return ret;
  }
};

namespace fs = std::filesystem;
using namespace std::chrono_literals;
// Key for the trie generator - must also store the time the file at path was
// modified
string genKey(string path) {
  fs::path p = path;
  auto ftime = fs::last_write_time(p);
  std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
  return path + std::asctime(std::localtime(&cftime));
}

//'n' indicates how many times the trie structure should be constructed in a
//workload
void runTrie(TrieGen& generator, int n, string outPath, bool cppersist) {
  vector<TrieNode*> toFree;
  Timer timer;
  timer.start();
  // Workload = convert dictionary ("words.txt") into a Trie data structure n
  // times.
  for (int i = 0; i < n; i++) {
    TrieNode* answer = generator.solve("words.txt");
    toFree.push_back(answer);
  }
  timer.end();
  for (vector<TrieNode*>::iterator it = toFree.begin(); it != toFree.end();
       it++) {
    TrieNode::freeAll(*it);
  }
  appendRowToFile(outPath, timer.getRow());
}

void runTrie(int input, string type, bool cppersist, bool recursive,
             bool keepCache) {
  string outPath =
      getOutPath("TrieTenSame", type, cppersist, recursive, keepCache);
  if (recursive) {
    if (!cppersist) {  // run workload on trie construction function, without
                       // cppersist applied.
      TrieGenerator gen;
      runTrie(gen, input, outPath, cppersist);
    } else {  // run workload on trie construction function, with cppersist
              // applied.
      auto genMemo = getLocalMemoizedObj<TrieGenerator>(
          genKey, TrieNode::pickle, TrieNode::unpickle, sha256);
      runTrie(genMemo, input, outPath, cppersist);
    }
  }
}

int main(int argc, char const* argv[]) {
  int numInput = stoi(argv[1]);
  bool cppersist = stoi(argv[2]);
  bool recursive = stoi(argv[3]);
  bool keepCache = stoi(argv[4]);
  int seed = stoi(argv[5]);
  const char* version = argv[6];
  runTrie(numInput, "", cppersist, recursive, keepCache);
  return 0;
}
