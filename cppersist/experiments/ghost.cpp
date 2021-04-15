#include <algorithm>
#include <filesystem>
#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "../local.hpp"
#include "../memory.hpp"
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
void completeTrie(TrieNode* head, string path) {
  std::ifstream infile(path);
  std::string line;
  while (std::getline(infile, line)) {
    std::istringstream iss(line);
    std::string word;
    iss >> word;
    addWordToTrie(word, head);
  }
}

class LeafSolver {
 public:
  virtual string solve(bool, TrieNode*) = 0;  // to reduce repeated code...
};

class LeafRec : public Memoizable<string, bool, TrieNode*>, public LeafSolver {
 public:
  // The ghost function that is memoized. p2 indicates which player it is
  // desired to win for, true = player 2, false = player 1. 'Cur' is the trie
  //node to begin search.
  string solve(bool p2, TrieNode* cur) override {
    if (cur->isLeaf()) {
      if (isRightLabel(p2, cur)) {
        return cur->prefix + ", ";  // Append this winning for p2
      }
      return "";  // We know we can't win...return empty string
    }

    if (!isRightLabel(p2, cur))
      return "";  // We know we can't win...return empty string

    string ans = "";  // the comma separated string of words to play for
    for (TrieNode*& child : cur->children) {
      if (child != nullptr) {
        string ret = solve(p2, child);
        if (ret != "") ans.append(ret);  // append winning strings
      }
    }
    return ans;  // the comma separated string of words to play for
  }

  // Returns if the 'check' trie node has a label that means the current player,
  // indicated by 'p2', will win
  bool isRightLabel(bool p2, TrieNode*& check) {
    if (check->label == '2')
      return p2 ? true : false;
    else
      return p2 ? false : true;
  }
};

class GhostSolver {
 public:
  GhostSolver(LeafSolver& solver_) : solver(solver_) {}
  string solve(string prefix,
               TrieNode* dict) {  // helper function to call our memoized search
    return solver.solve(
        prefix.length() == 1,
        dict->getNode(prefix));  // traverse to correct starting node
  }

 private:
  LeafSolver& solver;
};

string ghostKey(bool p2, TrieNode* curNode) {
  if (curNode->isRoot)
    return "root_node";  // only if we desire to start search from the empty
                         // string...can never be player two
  return curNode->prefix +
         to_string(p2);  // key is indicated by which player we want to win and
                         // the current prefix to search from
}

// Generated all the valid prefixes from 'head'
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

// Run the experiments
void runGhost(GhostSolver& solver, vector<string>& input, TrieNode* dict,
              string path, bool cppersist) {
  Timer timer;
  for (vector<string>::iterator it = input.begin(); it != input.end(); it++) {
    timer.start();
    string ans = solver.solve(*it, dict);
    timer.end();
  }
  appendRowToFile(path, timer.getRow());
}

// Memoization decision that only memoizes calls to nodes that are root or
// 1-level higher than the root
bool decision(bool p2, TrieNode* cur) {
  int length = cur->prefix.length();
  if (length > 2 || (p2 && (cur->label == '1') || (!p2 && (cur->label == '2'))))
    return false;
  return true;
}

void runGhost(vector<string>& input, TrieNode* dict, string type,
              bool cppersist, bool recursive, bool keepCache) {
  string path =
      getOutPath("GhostSearchNodes", type, cppersist, recursive, keepCache);
  if (recursive) {
    LeafRec noMemo;
    auto hasMemo = getLocalMemoizedObj<LeafRec>(
        ghostKey, identity<string>, identity<string>, identity<string>);
    hasMemo.setDecision(decision);  // set the memoization decision
    if (!cppersist) {               // run workload w/o cppersist
      GhostSolver rec(noMemo);
      runGhost(rec, input, dict, path, cppersist);
    } else {  // run workload w cppersist
      GhostSolver rec(hasMemo);
      runGhost(rec, input, dict, path, cppersist);
    }
  }
}

void runGhostWRep(vector<string>& input, TrieNode* dict, bool cppersist,
                  bool recursive, bool keepCache, int seed) {
  runGhost(input, dict, "WRep", cppersist, recursive, keepCache);
}

// Preprocess the trie - post order traversal
void postOrder(TrieNode* cur) {
  if (cur->isLeaf()) {  // it is a word in our dictionary
    if ((cur->prefix.length() % 2) == 0)
      cur->label = '1';  // p1 wins from here
    else
      cur->label = '2';  // p2 wins from here
    return;
  }
  for (TrieNode*& child : cur->children) {
    if (child != nullptr) {
      postOrder(child);  // post order traverse the children
    }
  }

  bool allSame =
      true;  // check if all the children of this node all have the same label
  char prev = '-';
  for (TrieNode*& child : cur->children) {
    if (child != nullptr) {
      char curC = child->label;
      if (prev == '-')
        prev = curC;
      else if (prev != curC) {
        allSame = false;
        break;
      }
    }
  }

  if (allSame)
    cur->label = prev;  // all the children of this node have the same label, so
                        // set this node's label to their label
  else {
    if ((cur->prefix.length() % 2) == 0)
      cur->label = '1';  // it is p1's turn at this node, therefore they can
                         // play towards one of their
    // children with a label corresponding to p1 winning

    else
      cur->label = '2';  // it is p2's turn at this node, therefore they can
                         // play towards one of their
    // children with a label corresponding to p2 winning
  }
}

int main(int argc, char const* argv[]) {
  int numInput = stoi(argv[1]);
  bool cppersist = stoi(argv[2]);
  bool recursive = stoi(argv[3]);
  bool keepCache = stoi(argv[4]);
  int seed = stoi(argv[5]);
  const char* version = argv[6];

  TrieNode* head = new TrieNode(false);
  head->isRoot = true;
  completeTrie(head, "./words.txt");
  postOrder(head);                                 // preprocess the trie
  vector<string> validPref = validPrefixes(head);  // get all valid prefixes
  vector<string> inp;
  srand(seed);
  while (
      inp.size() !=
      numInput) {  // randomly add 'numInput' valid prefixes for this workload
    inp.push_back(validPref[rand() % validPref.size()]);
  }
  runGhostWRep(inp, head, cppersist, recursive, keepCache, seed);
  TrieNode::freeAll(head);  // free the trie!
}
