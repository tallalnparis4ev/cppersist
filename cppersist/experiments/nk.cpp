#include <chrono>
#include <algorithm>
#include <iostream>
// #include <unordered_map>
#include <map>
#include <functional>
#include <vector>
#include <algorithm>
#include <random>

#include "../local.hpp"
#include "../utils/files.hpp"
#include "helpers/bigint.cpp"
typedef unsigned long long largestUnsigned;
using namespace std::chrono;
using namespace std;
using namespace cpst;
#define nano 1000000000
#define nMax 2000
// #define size (nMax+1)
// Return nCk 
// bigint binomialCoeff(int n, int k, bool seen[size][size], bigint memo[size][size]) 
// { 
//   if(n==k || k==0) return 1;
//   if(seen[n][k]) return memo[n][k];
//   bigint C[k + 1]; 
//   for(int i=0;i<k+1;i++) C[i] = 0;

//   C[0] = 1; // nC0 is 1 

//   for (int i = 1; i <= n; i++) { 
//       // Compute next row of pascal triangle using 
//       // the previous row 
//       for (int j = min(i, k); j > 0; j--) 
//           C[j] = C[j] + C[j - 1]; 
//   } 
//   seen[n][k] = true;
//   memo[n][k] = C[k];
//   return C[k]; 
// } 

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const
    {
      string combined = pair.first + "," + pair.second;
      return std::hash<string>()(combined);
      // return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

// bigint binomialCoeff(int n, int k, unordered_map<pair<int,int>,bigint,pair_hash>& memo) 
// { 
//     auto curPair = pair(n,k);
//     if(n==k || k==0) return 1;
//     if (memo.find(curPair) != memo.end()) return memo[curPair];

//     bigint answer = binomialCoeff(n-1,k-1,memo) + binomialCoeff(n-1,k,memo);
//     memo[curPair] = answer;
//     return answer;
// }

// bigint binomialCoeff(int n, int k, unordered_map<pair<int,int>,bigint>& memo) 
// { 
//     auto curPair = pair(n,k);
//     if(n==k || k==0) return 1;
//     if (memo.find(curPair) != memo.end()) return memo[curPair];

//     bigint answer = binomialCoeff(n-1,k-1,memo) + binomialCoeff(n-1,k,memo);
//     memo[curPair] = answer;
//     return answer;
// }

// class BinomialCoeff : public PersistentMemoizable<bigint,int,int>{
//   public:
//     bigint solve(int n, int k) override{
//       if(n==k || k==0) return 1;
//       return solve(n-1,k-1) + solve(n-1,k);
//     }
// };

string binKey(int n, int k){
  return to_string(n) + "," + to_string(k);
}

string binPickle(bigint result){
  return result.to_string();
}

bigint binUnpickle(string result){
  return bigint(result);
}

string binHash(string key){return key;}


// void runBinSeq(vector<pair<int, int>>& NKs, string outputFile){
//   // unordered_map<pair<int,int>,bigint,pair_hash> memo;
//   unordered_map<pair<int,int>,bigint,pair_hash> memo;
//   largestUnsigned totalTimeUnmemoized = 0;
//   for (vector<pair<int, int>>::iterator it = NKs.begin(); it != NKs.end(); it++){
//     auto start = high_resolution_clock::now();
//     bigint answer = binomialCoeff(it->first,it->second,memo);
//     auto timeTaken = duration_cast<nanoseconds>(high_resolution_clock::now()-start).count();
//     totalTimeUnmemoized += timeTaken;
//   }

//   largestUnsigned totalTimeMemoized = 0;
//   auto localMemo = 
//     getLocalMemoizedObj<BinomialCoeff>(binKey,binPickle,binUnpickle,"binTest",binHash);
//   for (vector<pair<int, int>>::iterator it = NKs.begin(); it != NKs.end(); it++){
//     localMemo(it->first, it->second);
//     totalTimeMemoized += localMemo.solveTime;
//   }
//   string row = to_string(totalTimeUnmemoized) + ", " + to_string(totalTimeMemoized);
//   appendRowToFile(outputFile,row);
// }


void runBinWORep(vector<pair<int, int>>& NKs, int seed){
  shuffle(NKs.begin(), NKs.end(), default_random_engine(seed));
  // runBinSeq(NKs,"./data/binCoeffWORep.csv");
}

void runBinWRep(vector<pair<int, int>>& NKs, int seed){
  srand(seed);
  vector<pair<int,int>> newInp;
  while(newInp.size() != NKs.size()){
    pair<int,int> toAdd = NKs[rand()%NKs.size()];
    newInp.push_back(toAdd);
  }
  // runBinSeq(NKs,"./data/binCoeffWRep.csv");
}

vector<pair<int,int>> generatePairs(int n){
  vector<pair<int,int>> ret;
  for(int i=0;i<=n;i++){
    for(int j=0;j<=i;j++){
      ret.push_back(pair(i,j));
    }
  }
  return ret;
}

int getIndex(int n, int k){
  int start = 0;
  if(n != 0){
    for(int i=1;i<=n;i++)
      start+=i;
  }
  return start+k;
}

bigint binomialCoeff(int n, int k, bigint memo[31626]) 
{ 
    auto curPair = pair(n,k);
    if(n==k || k==0) return 1;
    
    int index = getIndex(n,k);
    bigint maybeAnswer = memo[index];
    if (maybeAnswer != 0) return maybeAnswer;

    bigint answer = binomialCoeff(n-1,k-1,memo) + binomialCoeff(n-1,k,memo);
    memo[index] = answer;
    return answer;
}

int main(int argc, char const *argv[])
{ 
  vector<pair<int,int>> NKs = generatePairs(nMax);
  cout << NKs.size() << endl;
  bigint memo[2003001] = {0};
  largestUnsigned totalTimeUnmemoized = 0;
  bigint lastanswer = 0;
  for (vector<pair<int, int>>::iterator it = NKs.begin(); it != NKs.end(); it++){
    auto start = high_resolution_clock::now();
    bigint answer = binomialCoeff(it->first,it->second,memo);
    auto timeTaken = duration_cast<nanoseconds>(high_resolution_clock::now()-start).count();
    lastanswer = answer;
    totalTimeUnmemoized += timeTaken;
  }
  cout << totalTimeUnmemoized << endl;
  cout << lastanswer << endl;

  // cout << NKs.size() << endl;
  // int seed = stoi(argv[1]);
  // runBinWORep(NKs,seed);
  // runBinWRep(NKs,seed);
  // runBinSeq(NKs,"./data/binCoeffSeq.csv");
  return 0; 
} 


// void runTTTWORep(Board boards[NUM_TTT_BOARDS], int seed){
//   vector<int> remaining;
//   srand(seed);
//   for(int i=0;i<NUM_TTT_BOARDS;i++) remaining.push_back(i);
//   int index = 0;
//   Board newBoardsOrder[NUM_TTT_BOARDS];
//   while(!remaining.empty()){
//     int randomIndex = rand() % remaining.size();
//     Board newBoard(boards[remaining[randomIndex]].board);
//     newBoardsOrder[index++] = newBoard;
//     remaining.erase(remaining.begin() + randomIndex);
//   }
//   runTTTSeq(newBoardsOrder,"./data/TTTWORep.csv");
// }

// void runTTTWRep(Board boards[NUM_TTT_BOARDS], int seed){
//   srand(seed);
//   int index = 0;
//   Board newBoardsOrder[NUM_TTT_BOARDS];
//   for(int i=0;i<NUM_TTT_BOARDS;i++){
//     Board newBoard(boards[rand() % NUM_TTT_BOARDS].board);
//     newBoardsOrder[index++] = newBoard;
//   }
//   runTTTSeq(newBoardsOrder,"./data/TTTWRep.csv");
// }


// Return the number of array that can be 
// formed of size n and sum equals to k. 
// bigint countArray(int N, int K) 
// { 
//     return binomialCoeff(K - 1, N - 1); 
// } 
