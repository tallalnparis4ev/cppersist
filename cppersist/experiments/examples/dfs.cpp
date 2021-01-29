#include "../../local.hpp"
#include "../../mongo.hpp"
#include <iostream>
#include <list>

#include "../local.hpp"
#define STOP "A "
#define DELIMITER " "
using namespace std;
using namespace cpst;
class Node {
 public:
  int label;
  Node* left;
  Node* right;
};

class DFSSolver : public PersistentMemoizable<int, Node*, int> {
 public:
  int solve(Node* root, int find) override {
    if (root == NULL) return 0;
    if (root->label == find) return 1;
    return solve(root->left, find) == 1 || solve(root->right, find) == 1 ? 1
                                                                         : 0;
  };
};

Node* newNode(int label) {
  Node* node = (Node*)malloc(sizeof(Node));
  node->label = label;
  node->left = node->right = NULL;
  return (node);
}

void serialize(Node* root, string& tree) {
  if (root == NULL) {
    tree.append(STOP);
    return;
  }

  tree.append(to_string(root->label) + " ");
  serialize(root->left, tree);
  serialize(root->right, tree);
}

Node* deSerialize(string& tree) {
  if (tree.compare("") == 0) {
    return NULL;
  }
  if (tree.at(0) == 'A') {
    tree.erase(0, 2);
    return NULL;
  }
  int indNumEnd = tree.find(DELIMITER) + 1;
  int label = stoi(tree.substr(0, indNumEnd));
  tree.erase(0, indNumEnd);
  Node* root = newNode(label);
  root->left = deSerialize(tree);
  root->right = deSerialize(tree);
  return root;
}

string keymaker(Node* root, int find) {
  string tree = "";
  serialize(root, tree);
  return tree + "/" + to_string(find);
}

int strtoi(string x) { return std::stoi(x); }
string intostr(int x) { return std::to_string(x); }

string keymaker2(int n) { return std::to_string(n); }

int main() {
  string tree = "1 2 A A 3 A A ";
  Node* root = deSerialize(tree);
  PersistentMemoized memoizedFib = getLocalMemoizedObj<DFSSolver>(LRU_CACHE,keymaker,intostr,strtoi);
  int z = memoizedFib(root,3);
  cout << z << endl;
}

// #include <future>
// #include <mutex>
// #include <thread>         // std::this_thread::sleep_for
// #include <chrono>
// #include <iostream>
// using namespace std;
// namespace x{
//   string x;
// }
// namespace cppst{

// }
// std::future<void> test;
// void sleeper(){
//   cout << "STARTED" << endl;
//   std::this_thread::sleep_for (std::chrono::seconds(10));
// };
// void external(){
//   test = std::async(&sleeper);
// }
// int main(int argc, char const *argv[])
// {
//   external();
//   // std::future<void> writeRes = std::async(std::launch::async,&PersistentMemoized<T,Ret,Args...>::write,this,args..., realAnswer);
//   cout << "DONE" << endl;
//   /* code */
//   return 0;
// }
