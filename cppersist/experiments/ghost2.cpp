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

bool isP2Win(TrieNode* cur) { return (cur->length() % 2) == 1; }

struct Result {
  string word;
  bool p1Win;
  string getWord() { 
    return word;
  }
  void print() {
    string winner = p1Win ? "player one " : "player two ";
    string line = winner + "will win by playing towards " + word;
    cout << line << endl;
  }
  static string toString(Result res){
    string oneOrZero = res.p1Win ? "1" : "0";
    return oneOrZero + " " + res.getWord();
  }
  static Result fromString(string resStr){
    return Result{resStr.substr(2), resStr.at(0) == '1'};
  }
};

bool isValidPartial(string partial, TrieNode* head) {
  TrieNode* cur = head;
  for (char& c : partial) {
    TrieNode* child = cur->getChild(c);

    if (child == nullptr) {
      return false;
    }

    if (child->isWord) {
      return false;
    }

    cur = child;
  }
  return true;
}


class GhostSolver {
 public:
  virtual Result solve(string partial, string dictPath) = 0;
};


class GhostRec : public PersistentMemoizable<Result, string, string>,
                 public GhostSolver {
 public:
  Result solve(string partial, string dictPath) override {
    TrieNode* dict = new TrieNode(false);
    completeTrie(dict,dictPath);
    bool p1 = (partial.length() % 2) == 0;
    TrieNode* cur = dict->getNode(partial);
    if (cur->isWord){
      string prefix = cur->prefix;
      TrieNode::freeAll(dict);
      return Result{prefix, p1};
    }
    Result aLosingRes;
    for (TrieNode*& child : cur->children) {
      if (child != nullptr) {
        Result res = solve(child->prefix, dictPath);
        if (res.p1Win && p1) {
          TrieNode::freeAll(dict);
          return res;
        }
        if (!res.p1Win && !p1) {
          TrieNode::freeAll(dict);
          return res;
        }
        aLosingRes = res;
      }
    }
    TrieNode::freeAll(dict);
    return aLosingRes;
  }
};



// class GhostRec : public PersistentMemoizable<string, string, string>,
//                  public GhostSolver {
//  public:
//   string solve(string partial, string dictPath) override {
//     TrieNode* dict = new TrieNode(false);
//     completeTrie(dict,dictPath);
//     bool p1 = (partial.length() % 2) == 0;
//     TrieNode* searchStart = head->getNode(partial);
//     Result result = tryToWin(p1, searchStart);
//     if (result.p1Win != p1) {
//       return "You can't win";
//     }
//     return "Play for: " + result.getWord();
//   }
// };

namespace fs = std::filesystem;
using namespace std::chrono_literals;
string ghostKey(string partial, string path){
  fs::path p = path;
  auto ftime = fs::last_write_time(p);
  std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime); 
  return partial + path + std::asctime(std::localtime(&cftime));
}

vector<string> validPrefixes(TrieNode* head) {
  vector<string> prefixes;
  queue<TrieNode*> queue;
  queue.push(head);
  while (!queue.empty()) {
    TrieNode* cur = queue.front();
    queue.pop();
    if (!(cur->isWord)) {
      prefixes.push_back(cur->prefix);
      for (TrieNode*& child : cur->children) {
        if (child == nullptr) continue;
        queue.push(child);
      }
    }
  }
  return prefixes;
}

void runGhost(string& dictPath, GhostSolver& solver, vector<string>& input, 
              string outPath, bool cppersist) {
  Timer timer;
  timer.start();
  for (vector<string>::iterator it = input.begin(); it != input.end(); it++) {
    Result answer = solver.solve(*it, dictPath);
  }
  timer.end();
  appendRowToFile(outPath, timer.getRow());
}

void runGhost(string& dictPath, vector<string>& input, string type,
              bool cppersist, bool recursive, bool keepCache) {
  string outPath = getOutPath("Ghost2", type, cppersist, recursive, keepCache);
  if (recursive) {
    if (!cppersist) {
      GhostRec rec;
      runGhost(dictPath, rec, input, outPath, cppersist);
    } 
    else {
      auto localMemo = getLocalMemoizedObj<GhostRec>(
        ghostKey, Result::toString, Result::fromString, sha256);  
      runGhost(dictPath, localMemo, input, outPath, cppersist);
    }
  }
}

void runGhostWORep(string& dictPath, vector<string>& input, bool cppersist,
                   bool recursive, bool keepCache, int seed) {
  shuffle(input.begin(), input.end(), default_random_engine(seed));
  runGhost(dictPath, input, "WORep", cppersist, recursive, keepCache);
}

void runGhostWRep(string& dictPath, vector<string>& input, bool cppersist,
                  bool recursive, bool keepCache, int seed) {
  srand(seed);
  vector<string> newInp;
  while (newInp.size() != input.size()) {
    newInp.push_back(input[rand() % input.size()]);
  }
  runGhost(dictPath, newInp, "WRep", cppersist, recursive, keepCache);
}


int main(int argc, char const* argv[]) {


  int numInput = stoi(argv[1]);
  bool cppersist = stoi(argv[2]);
  bool recursive = stoi(argv[3]);
  bool keepCache = stoi(argv[4]);
  int seed = stoi(argv[5]);
  const char* version = argv[6];
  string dictPath = "./words.txt";
  TrieNode* head = new TrieNode(false);
  completeTrie(head,dictPath);
  vector<string> validPref = validPrefixes(head);
  if (std::strcmp(version, "worep") == 0) {
    runGhostWORep(dictPath,validPref, cppersist, recursive, keepCache, seed);
  }

  if (std::strcmp(version, "wrep") == 0) {
    runGhostWRep(dictPath,validPref, cppersist, recursive, keepCache, seed);
  }

  TrieNode::freeAll(head);

  return 0;
}
