#define ALPHABET_SIZE 26
#define ALPHABET 'A'
#include <algorithm>
#include <array>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
class TrieNode {
 public:
  char label = '_';  // indicates if this is a goal node for p1 ('1') or p2
                     // ('2'). '_' indicates we don't know yet.
  bool isLeaf() {
    return this->isWord;  // leaf nodes are those that correspond ot a word in a
                          // dictionary
  }
  // serialize the whole trie structure
  static string pickle(TrieNode* root) {
    if (root == nullptr) return "";

    string answer = "<";
    for (int i = 0; i < ALPHABET_SIZE; i++) {
      TrieNode*& child = root->children[i];
      if (child == nullptr) continue;
      char letter = i + ALPHABET;
      if (child->isWord) answer += '-';
      answer += letter;
      answer += pickle(child);
    }
    answer += ">";
    return answer;
  }

  // deserialize the whole trie structure
  static TrieNode* unpickle(string data) {
    if (data.length() == 0) return nullptr;

    TrieNode* root = new TrieNode(false);
    TrieNode* current = root;
    bool isWord = false;
    std::stack<TrieNode*> path;
    for (char& c : data) {
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
          current = new TrieNode(pathTop->prefix + c);
          current->isWord = isWord;
          isWord = false;
          pathTop->setChild(c, current);
      }
    }
    return root;
  }

  std::string prefix = "";  // prefix to get to this trie
  bool isRoot = false;      // true if this is the root of the whole trie

  // constructors
  TrieNode(string prefix) { this->prefix = prefix; }
  TrieNode(bool isWord) { this->isWord = isWord; }

  // The children of this trie - all init to null
  std::array<TrieNode*, ALPHABET_SIZE> children = {};

  // Is the prefix from the root node to this trie a word in our dictionary?
  bool isWord;

  // Get the child of this node corresponding to the character 'child'
  TrieNode* getChild(char& child) { return children[child - ALPHABET]; }

  // Setter for a child of this node - and the character to transition to this
  // child
  void setChild(char& child, TrieNode* childNode) {
    children[child - ALPHABET] = childNode;
  }

  // Print methods for the trie ----
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
  // -----

  // From this node, traverse 'prefix' and return the corresponding trie node.
  TrieNode* getNode(string prefix) {
    TrieNode* cur = this;
    for (char& c : prefix) {
      TrieNode* child = cur->getChild(c);
      if (child == nullptr) return nullptr;
      cur = child;
    }
    return cur;
  }

  // Get the length of the prefix at this trie
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

  // Free all the trie nodes with 'cur' as the root
  static void freeAll(TrieNode* cur) {
    list<TrieNode*> toDelete;
    toDelete.push_back(cur);
    while (!toDelete.empty()) {
      TrieNode* enumerate = toDelete.front();
      toDelete.pop_front();
      for (TrieNode*& child : enumerate->children) {
        if (child != nullptr) toDelete.push_back(child);
      }
      delete enumerate;
    }
  }
};