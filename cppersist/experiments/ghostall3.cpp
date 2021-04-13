#include <algorithm>
#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

#include "../local.hpp"
#include "../memory.hpp"
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

class LeafSolver{
  public:
    virtual string solve(bool,TrieNode*) = 0;
};

class LeafRec : public Memoizable<string,bool,TrieNode*>, public LeafSolver{
  public:
    string solve(bool p2, TrieNode* cur) override{
      if(cur->isLeaf()){
        if(isRightLabel(p2,cur)){
          return cur->prefix + ", ";
        }
        return "";
      }

      if(!isRightLabel(p2,cur)) return "";
      
      string ans = "";
      for (TrieNode*& child : cur->children) {
        if(child!=nullptr){
          string ret = solve(p2,cur);
          if(ret!="") ans.append(ret);
        }
      }
      return ans;
    }

    bool isRightLabel(bool p2, TrieNode*& check){
      if(check->label == '2') return p2 ? true : false;
      else return p2 ? false : true;
    }
};

class GhostSolver{
 public:
  GhostSolver(LeafSolver& solver_) : solver(solver_) {}
  string solve(string prefix, TrieNode* dict) {
    return solver.solve(prefix.length()==1,dict->getNode(prefix));
  }
  private:
    LeafSolver& solver;
};

//Explore smaller words, maybe make the dictionairy bigger?
//Change the computational problem to calculate the Trie each time
//While generating prefixes give higher weighting to smaller length words

string ghostKey(bool p2, TrieNode* curNode) 
{ 
  if(curNode->isRoot) return "root_node";
  return curNode->prefix+to_string(p2); 
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

bool decision(bool p2, TrieNode* cur){
  int length = cur->prefix.length();
  if(length>2 || (p2 && (cur->label == '1') || (!p2  && (cur->label == '2')))) return false;
  return true;
  // if(p2) return (length%2) == 1;
  // return (length%2) == 0;
  // cur->prefix % 2 == 0 when its p1's turn
  // cur->prefix % 2 == 1 when its p2's turn
}

void runGhost(vector<string>& input, TrieNode* dict, string type,
              bool cppersist, bool recursive, bool keepCache) {
  string path = getOutPath("GhostSearchNodes", type, cppersist, recursive, keepCache);
  if (recursive) {
    LeafRec noMemo; 
    auto hasMemo = getLocalMemoizedObj<LeafRec>(
        ghostKey, identity<string>, identity<string>, identity<string>);
    hasMemo.setDecision(decision);
    if (!cppersist) {
      GhostSolver rec(noMemo);
      runGhost(rec, input, dict, path, cppersist);
    } else {
      GhostSolver rec(hasMemo);
      runGhost(rec, input, dict, path, cppersist);
    }
  }
}

void runGhostWORep(vector<string>& input, TrieNode* dict, bool cppersist,
                   bool recursive, bool keepCache, int seed) {
  // shuffle(input.begin(), input.end(), default_random_engine(seed));
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

void postOrder(TrieNode* cur){
  if(cur->isLeaf()){
    if((cur->prefix.length() % 2) == 0) cur->label = '1';
    else cur->label = '2';
    return;
  }
  for (TrieNode*& child : cur->children){
    if(child!=nullptr){
      postOrder(child);
    }
  }

  bool allSame = true;
  char prev = '-';
  for (TrieNode*& child : cur->children){
    if(child!=nullptr){
      char curC = child->label;
      if(prev == '-') prev = curC;
      else if(prev != curC){
        allSame = false;
        break;
      }
    }
  }
  
  if(allSame) cur->label = prev;
  else{
    if((cur->prefix.length() % 2) == 0) cur->label = '1';
    else cur->label = '2';
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
  completeTrie(head,"./words.txt");
  postOrder(head);
  vector<string> validPref = validPrefixes(head);
  vector<string> inp;
  srand(seed);
  while(inp.size() != numInput) {
    inp.push_back(validPref[rand() % validPref.size()]);
  }
  runGhostWORep(inp, head, cppersist, recursive, keepCache, seed);
  TrieNode::freeAll(head);

  // if (std::strcmp(version, "worep") == 0) {
  //   runGhostWORep(validPref, head, cppersist, recursive, keepCache, seed);
  // }

  // if (std::strcmp(version, "wrep") == 0) {
  //   runGhostWRep(validPref, head, cppersist, recursive, keepCache, seed);
  // }


  // return 0;
}
