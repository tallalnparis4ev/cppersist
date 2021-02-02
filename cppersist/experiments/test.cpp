#include "../local.hpp"
#include "../mongo.hpp"
#include "../utils/log.hpp"
#include "data-generation/generators.cpp"
#include "../utils/files.hpp"
#include "primetests.cpp"
#include "fibtests.cpp"
#include <string>
#include <set>
#include <iostream>
using namespace std::chrono;
using namespace cpst;
using namespace std;
#define NUM_INPUT 100000


// Node* treeGenerator(){
//   int label = 0;
//   Node* start = new Node(label++);
//   list<Node*> listOfNodes;
//   listOfNodes.push_front(start);
//   for(int i=0;i<100000;i++){
//     Node* cur = listOfNodes.front();
//     listOfNodes.pop_front();
//     Node* left = new Node(label++);
//     Node* right = new Node(label++);
//     listOfNodes.push_back(left);
//     listOfNodes.push_back(right);
//     cur->left = left;
//     cur->right = right;
//   }
//   return start;
// }
int main(int argc, char const *argv[])
{ 
  // cout << countTrees(5) << endl;
  // auto start = high_resolution_clock::now();
  // string tree = "1 2 3 4 5 6 7 8 9 10 A A";
  // Node* root = treeDeserialize(tree);
  // Node* root = treeGenerator();
  // HeightSolver solver;
  // DFSSolverIterative solver; 
  // cout << solver.solve(root,3) << endl;
  // cout << solver.solve(root) << endl;
  // auto timeTaken = duration_cast<nanoseconds>(high_resolution_clock::now()-start).count();
  // cout << timeTaken << endl;
  // for(fibNumber i = 0 ;i < 2000000000000000; i++){
    // auto timeTaken = solve(i);
    // if(timeTaken > 127813){
      // cout << timeTaken << endl;
      // cout << i << endl;
      // break;
    // }
  // }

  // auto start = high_resolution_clock::now();
  // cout << listIntsToString(solve(INT_MAX-1)) << endl;
  // auto timeTaken = duration_cast<nanoseconds>(high_resolution_clock::now()-start).count();
  // cout << timeTaken << endl;
  int input = stoi(argv[1]);
  // runFibTests(input);
  // runPrimesAlt2TestsSeq();
  // runPrimesAlt2TestWORep(input);
  runPrimesAlt2TestWRep(input);
  return 0;
}
