#include <chrono>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <functional>

#include "../local.hpp"
#include "../utils/files.hpp"
#include "helpers/bigint.cpp"
typedef unsigned long long largestUnsigned;
using namespace std::chrono;
using namespace std;
using namespace cpst;
#define nano 1000000000
#define nMax 2000
#define size (nMax+1)
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
      return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

bigint binomialCoeff(int n, int k, unordered_map<pair<int,int>,bigint,pair_hash>& memo) 
{ 
    auto curPair = pair(n,k);
    if(n==k || k==0) return 1;
    if (memo.find(curPair) != memo.end()) return memo[curPair];

    bigint answer = binomialCoeff(n-1,k-1,memo) + binomialCoeff(n-1,k,memo);
    memo[curPair] = answer;
    return answer;
}
  
class BinomialCoeff : public PersistentMemoizable<bigint,int,int>{
  public:
    bigint solve(int n, int k) override{
      if(n==k || k==0) return 1;
      return solve(n-1,k-1) + solve(n-1,k);
    }
};

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


void runBinSeq(list<pair<int, int>> NKs, string outputFile){
  unordered_map<pair<int,int>,bigint,pair_hash> memo;
  largestUnsigned totalTimeUnmemoized = 0;
  for (list<pair<int, int>>::iterator it = NKs.begin(); it != NKs.end(); it++){
    auto start = high_resolution_clock::now();
    bigint answer = binomialCoeff(it->first,it->second,memo);
    auto timeTaken = duration_cast<nanoseconds>(high_resolution_clock::now()-start).count();
    totalTimeUnmemoized += timeTaken;
  }

  largestUnsigned totalTimeMemoized = 0;
  auto localMemo = 
    getLocalMemoizedObj<BinomialCoeff>(binKey,binPickle,binUnpickle,"binTest",binHash);
  for (list<pair<int, int>>::iterator it = NKs.begin(); it != NKs.end(); it++){
    localMemo(it->first, it->second);
    totalTimeMemoized += localMemo.solveTime;
  }
  string row = to_string(totalTimeUnmemoized) + ", " + to_string(totalTimeMemoized);
  appendRowToFile(outputFile,row);
}

list<pair<int,int>> generatePairs(int n){
  list<pair<int,int>> ret;
  for(int i=0;i<=n;i++){
    for(int j=0;j<=i;j++){
      ret.push_back(pair(i,j));
    }
  }
  return ret;
}

// Driver Code 
int main() 
{ 
  list<pair<int,int>> NKs = generatePairs(nMax);
  runBinSeq(NKs,"./data/binCoeffSeq.csv");
  return 0; 
} 




// Return the number of array that can be 
// formed of size n and sum equals to k. 
// bigint countArray(int N, int K) 
// { 
//     return binomialCoeff(K - 1, N - 1); 
// } 
