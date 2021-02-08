#include "../local.hpp"
#include "../mongo.hpp"
#include "../utils/log.hpp"
#include "data-generation/generators.cpp"
#include "../utils/files.hpp"
// #include "primetests.cpp"
// #include "fibtests.cpp"
#include "fibbiginttests.cpp"
#include <string>
#include <set>
#include <iostream>
// #include "helpers/bigint.cpp"
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

bigint testDontUse(int n){
  if(n==0) return 0;
  if(n==1) return 1;
  bigint prev = 0;
  bigint cur = 1;
  while(n>=2){
    bigint sum = prev + cur;
    prev = cur;
    cur = sum;
    n--;
  }
  return cur;
}

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
  // cout << testDontUse(100000) << endl;
  // auto timeTaken = duration_cast<nanoseconds>(high_resolution_clock::now()-start).count();
  // cout << timeTaken << endl;
  //0,1,1,2,3,5
  // cout << "HEY" << endl;
  // int main(){
	// ios::sync_with_stdio(0), cin.tie(0);
	// bigint a=99999999;
	// a*=1000200000003000LL;
	// cout<<a<<'\n';
// }
  int input = stoi(argv[1]);
  // runFibBITestSeq();
  // runFibBITestWORep(input);
  runFibBITestWRep(input);
  // runFibTests(input);
  // runPrimesAlt2TestsSeq();
  // runPrimesAlt2TestWORep(input);
  // runPrimesAlt2TestWRep(input);
  // runFibTestWORep(input);
  return 0;
}
