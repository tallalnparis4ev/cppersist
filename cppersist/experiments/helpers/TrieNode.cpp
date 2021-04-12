#define ALPHABET_SIZE 26
#define ALPHABET 'A'
#include <algorithm>
#include <array>
#include <iostream>
#include <queue>
#include <string>
#include <stack>
class TrieNode {
 public:
  char label = '_'; //indicates if this is a goal for p1 or p2
  bool isLeaf(){
    return this->isWord;
    // for (int i = 0; i < ALPHABET_SIZE; i++) 
      // if (this->children[i] != nullptr) return false;
    // return true;
  }
  static string pickle(TrieNode* root) {
    if (root == nullptr)
        return "";

    string answer = "<";
    for (int i=0;i<ALPHABET_SIZE;i++) {
      TrieNode*& child = root->children[i];
      if (child == nullptr) continue;
      char letter = i + ALPHABET;
      if(child->isWord) answer += '-';
      answer += letter;
      answer += pickle(child);
    }
    answer += ">";
    return answer;
  }

  static TrieNode* unpickle(string data) {
      if (data.length() == 0)
          return nullptr;

      TrieNode* root = new TrieNode(false);
      TrieNode* current = root;
      bool isWord = false;
      std::stack<TrieNode*> path;
      for (char &c: data) {
          switch (c) {
          case '<':
              path.push(current);
              break;
          case '>':
              path.pop();
              break;
          case '-':
              isWord = true;
              break;
          default:
              TrieNode* pathTop = path.top();
              current = new TrieNode(pathTop->prefix+c);
              current->isWord = isWord;
              isWord = false;
              pathTop->setChild(c, current);
          }
      }
      return root;
  }
  
  std::string prefix = "";
  bool isRoot = false;
  TrieNode(string prefix) { this->prefix = prefix; }
  TrieNode(bool isWord) { this->isWord = isWord; }
  std::array<TrieNode*, ALPHABET_SIZE> children = {};
  bool isWord;
  TrieNode* getChild(char& child) { return children[child - ALPHABET]; }
  void setChild(char& child, TrieNode* childNode) {
    children[child - ALPHABET] = childNode;
  }
  void print() {
    std::stack<TrieNode*> stack;
    stack.push(this);
    while (!stack.empty()) {
      TrieNode* curNode = stack.top();
      stack.pop();
      if (curNode->isWord) {
        cout << curNode->prefix << endl;
      }
      for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (curNode->children[i] != nullptr) {
          stack.push(curNode->children[i]);
        }
      }
    }
  }
  void printPaths() {
    std::stack<TrieNode*> stack;
    stack.push(this);
    while (!stack.empty()) {
      TrieNode* curNode = stack.top();
      stack.pop();
      if (curNode->isWord) {
        cout << curNode->prefix << endl;
      }
      for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (curNode->children[i] != nullptr) {
          stack.push(curNode->children[i]);
        }
      }
    }
  }
  TrieNode* getNode(string prefix) {
    TrieNode* cur = this;
    for (char& c : prefix) {
      TrieNode* child = cur->getChild(c);
      if (child == nullptr) return nullptr;
      cur = child;
    }
    return cur;
  }
  int length() { return prefix.length(); }
  static int getHeight(TrieNode* cur) { return getHeight(cur, 0); }
  static int getHeight(TrieNode* cur, int height) {
    int max = -1;
    for (TrieNode*& child : cur->children) {
      if (child != nullptr) {
        max = std::max(max, getHeight(child, height + 1));
      }
    }
    return std::max(max, height);
  }

  static void freeAll(TrieNode* cur){
    list<TrieNode*> toDelete;
    toDelete.push_back(cur);
    while(!toDelete.empty()){
      TrieNode* enumerate = toDelete.front();
      toDelete.pop_front();
      for (TrieNode*& child : enumerate->children) {
        if(child != nullptr)
          toDelete.push_back(child);
      }
      delete enumerate;
    }
  }
};