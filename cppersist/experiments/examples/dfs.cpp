#include "../../local.hpp"
#include "../../mongo.hpp"
#include <iostream>
#include <list>
#include <algorithm>

#include "../local.hpp"
#define STOP "A "
#define DELIMITER " "
#define NODE "N "
using namespace std;
using namespace cpst;
class Node {
 public:
  int label;
  Node* left = NULL;
  Node* right = NULL;
  int height = 0;
  Node(int label):label(label){}
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

class DFSSolverIterative : public PersistentMemoizable<int, Node*, int> {
 public:
  int solve(Node* root, int find) override {
    std::list<Node*> nodes;
    nodes.push_back(root);
    while (!nodes.empty())
    {
      Node* cur = nodes.front();
      nodes.pop_front();
      if(cur->label == find) return 1;
      nodes.push_back(cur->left);
      nodes.push_back(cur->right);
    }
    return 0;
  };
};


class HeightSolver : public PersistentMemoizable<int, Node*> {
  public:
  int solve(Node* root) override {
    std::list<Node*> nodes;
    int max = 0;
    nodes.push_back(root);
    root->height = 0;
    while (!nodes.empty())
    {
      Node* cur = nodes.front();
      nodes.pop_front();
      max = std::max(cur->height,max);
      if(cur->left != NULL){
        cur->left->height = cur->height+1;
        nodes.push_back(cur->left);
      }
      if(cur->right != NULL){
        cur->right->height = cur->height+1;
        nodes.push_back(cur->right);
      }
    }
    return max;
  };
};

Node* newNode(int label) {
  Node* node = (Node*)malloc(sizeof(Node));
  node->label = label;
  node->left = node->right = NULL;
  return (node);
}

void treeSerialize(Node* root, string& tree) {
  if (root == NULL) {
    tree.append(STOP);
    return;
  }

  tree.append(to_string(root->label) + " ");
  treeSerialize(root->left, tree);
  treeSerialize(root->right, tree);
}

void treeSerializeStructure(Node* root, string& tree) {
  if (root == NULL) {
    tree.append(STOP);
    return;
  }
  tree.append(NODE);
  treeSerializeStructure(root->left, tree);
  treeSerializeStructure(root->right, tree);
}

Node* treeDeserialize(string& tree) {
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
  root->left = treeDeserialize(tree);
  root->right = treeDeserialize(tree);
  return root;
}

string bfsKeyMaker(Node* root, int find) {
  string tree = "";
  treeSerialize(root, tree);
  return tree + "/" + to_string(find);
}

int bfsUnpickle(string x) { return std::stoi(x); }
string bfsPickle(int x) { return std::to_string(x); }

// int main() {
  // string tree = "1 2 A A 3 A A ";
  // Node* root = deSerialize(tree);
  // PersistentMemoized memoizedFib = getLocalMemoizedObj<DFSSolver>(LRU_CACHE,keymaker,intostr,strtoi);
  // int z = memoizedFib(root,3);
  // cout << z << endl;
// }

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
