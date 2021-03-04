#include <algorithm>
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
void completeTrie(TrieNode* head) {
  std::ifstream infile("words.txt");
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
  TrieNode* lastNode;
  bool p1Win;
  string getWord() { return lastNode->prefix; }
  void print() {
    string winner = p1Win ? "player one " : "player two ";
    string line = winner + "will win by playing towards " + lastNode->prefix;
    cout << line << endl;
  }
};

Result tryToWin(bool p1, TrieNode* cur) {
  if (cur->isWord) return Result{cur, p1};

  Result aLosingRes;

  for (TrieNode*& child : cur->children) {
    if (child != nullptr) {
      Result res = tryToWin(!p1, child);
      if (res.p1Win && p1) {
        return res;
      }
      if (!res.p1Win && !p1) {
        return res;
      }
      aLosingRes = res;
    }
  }
  return aLosingRes;
}

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
  virtual string solve(string partial, TrieNode* head) = 0;
};

class GhostRec : public PersistentMemoizable<string, string, TrieNode*>,
                 public GhostSolver {
 public:
  string solve(string partial, TrieNode* head) override {
    bool p1 = (partial.length() % 2) == 0;
    TrieNode* searchStart = head->getNode(partial);
    Result result = tryToWin(p1, searchStart);
    if (result.p1Win != p1) {
      return "You can't win";
    }
    return "Play for: " + result.getWord();
  }
};

string ghostKey(string partial, TrieNode* ignored) { return partial; }

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

void runGhost(GhostSolver& solver, vector<string>& input, TrieNode* dict,
              string path, bool cppersist) {
  Timer timer;
  timer.start();
  for (vector<string>::iterator it = input.begin(); it != input.end(); it++) {
    string answer = solver.solve(*it, dict);
  }
  timer.end();
  appendRowToFile(path, timer.getRow());
}

void runGhost(vector<string>& input, TrieNode* dict, string type,
              bool cppersist, bool recursive, bool keepCache) {
  string path = getOutPath("Ghost", type, cppersist, recursive, keepCache);
  if (recursive) {
    GhostRec rec;
    auto localMemo = getLocalMemoizedObj<GhostRec>(
        ghostKey, identity<string>, identity<string>, identity<string>);
    if (!cppersist) {
      runGhost(rec, input, dict, path, cppersist);
    } else {
      runGhost(localMemo, input, dict, path, cppersist);
    }
  }
}

void runGhostWORep(vector<string>& input, TrieNode* dict, bool cppersist,
                   bool recursive, bool keepCache, int seed) {
  shuffle(input.begin(), input.end(), default_random_engine(seed));
  runGhost(input, dict, "WORep", cppersist, recursive, keepCache);
}

void runGhostWRep(vector<string>& input, TrieNode* dict, bool cppersist,
                  bool recursive, bool keepCache, int seed) {
  srand(seed);
  vector<string> newInp;
  while (newInp.size() != input.size()) {
    newInp.push_back(input[rand() % input.size()]);
  }
  runGhost(newInp, dict, "WRep", cppersist, recursive, keepCache);
}


int main(int argc, char const* argv[]) {


  int numInput = stoi(argv[1]);
  bool cppersist = stoi(argv[2]);
  bool recursive = stoi(argv[3]);
  bool keepCache = stoi(argv[4]);
  int seed = stoi(argv[5]);
  const char* version = argv[6];

  TrieNode* head = new TrieNode(false);
  completeTrie(head);
  vector<string> validPref = validPrefixes(head);
  if (std::strcmp(version, "worep") == 0) {
    runGhostWORep(validPref, head, cppersist, recursive, keepCache, seed);
  }

  if (std::strcmp(version, "wrep") == 0) {
    runGhostWRep(validPref, head, cppersist, recursive, keepCache, seed);
  }

  return 0;
}
