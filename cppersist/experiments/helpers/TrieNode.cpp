#define ALPHABET_SIZE 26
#include <algorithm>
#include <array>
#include <iostream>
#include <queue>
#include <string>
class TrieNode {
 public:
  std::string prefix = "";
  TrieNode(string prefix) { this->prefix = prefix; }
  TrieNode(bool isWord) { this->isWord = isWord; }
  std::array<TrieNode*, ALPHABET_SIZE> children = {};
  bool isWord;
  TrieNode* getChild(char& child) { return children[child - 'A']; }
  void setChild(char& child, TrieNode* childNode) {
    children[child - 'A'] = childNode;
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
};