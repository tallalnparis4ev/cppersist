#include <algorithm>
#include <chrono>
#include <iostream>
#include <array>
#include <vector>
#include <random>

#include "../local.hpp"
#include "../utils/files.hpp"
#include "./helpers/Timer.cpp"
#include <sstream>
typedef unsigned long long largestUnsigned;
using namespace std::chrono;
using namespace std;
using namespace cpst;
const int N = 4;
const int numCells = N*N;
const int invalidNumXs = (numCells%2 == 0) ? (numCells/2) : ((numCells/2) + 1);

struct Move {
  int row, col = 0;
  int score;
  static string toString(Move move){
    std::ostringstream oss;
    oss << move.row << move.col << move.score;
    return oss.str();
  }
  static Move fromString(string moveStr){
    return Move{moveStr.at(0) - '0', moveStr.at(1) - '0', std::stoi(moveStr.substr(2))};
  }
  bool isOtherBetter(const Move& other, bool isX){
    if(isX){
      return other.score > this->score;
    }
    return other.score < this->score;
  }
};

char player = 'x', opponent = 'o';

bool isMovesLeft(char board[N][N]) {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      if (board[i][j] == '_') return true;
  return false;
}

// struct Result{
//   bool someoneWon = false;
//   int score;
// }


int checkRowCol(char b[N][N], bool row){
  for(int i=0;i<N;i++){
    bool allSame = true;
    char prev = row ? b[i][0] : b[0][i];
    for(int j=1;j<N;j++){
      char cur = row ? b[i][j] : b[j][i];
      if(prev != cur || cur == '_'){
        allSame = false;
        break;
      }
    }
    if(allSame){
      if (prev == player)
        return +10;
      else if (prev == opponent)
        return -10;
    }
  }
  return 0;
}

int checkDiag(char b[N][N]){
  char prev = b[0][0];
  for(int i=1;i<N;i++){
    char cur = b[i][i];
    if(prev != cur || cur == '_') return 0;
  }
  if (prev == player)
    return +10;
  else if (prev == opponent)
    return -10;
  return 0;
}

int checkOtherDiag(char b[N][N]){
  char prev = b[0][N-1];
  for(int i=1;i<N;i++){
    char cur = b[i][N-(i+1)];
    if(prev != cur || cur == '_') return 0;
  }
  if (prev == player)
    return +10;
  else if (prev == opponent)
    return -10;
  return 0;
}

int evaluate(char b[N][N]) {
  int rows = checkRowCol(b,true);
  if(rows!=0) return rows;
  
  int cols = checkRowCol(b,false);
  if(cols!=0) return cols;
  
  int diag1 = checkDiag(b);
  if(diag1!=0) return diag1;
  
  int diag2 = checkOtherDiag(b);
  if(diag2!=0) return diag2;

  return 0;
}




int count(char curBoard[N][N], bool xs) {
  int xCount = 0;
  int oCount = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (curBoard[i][j] == 'x') xCount++;
      if (curBoard[i][j] == 'o') oCount++;
    }
  }
  return xs ? xCount : oCount;
}

bool isValidTTTBoard(char curBoard[N][N]) {
  int xCount = count(curBoard, true);
  int oCount = count(curBoard, false);
  return ( (xCount == oCount) || (oCount == (xCount-1)) ) 
            && (evaluate(curBoard) == 0) && (xCount != invalidNumXs);
}
void copyBoardTo(char from[N][N], char to[N][N]) {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) to[i][j] = from[i][j];
}

class Board {
 public:
  char board[N][N];
  Board(char board[N][N]) { copyBoardTo(board, this->board); }
  Board() {}
  void assign(int i, int j, char c){
    board[i][j] = c;
  }
  const char& get(int i, int j) const{
    return board[i][j];
  }
};

bool isXTurn(const Board& board){
  int xCount = 0;
  int oCount = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      const char& ref = board.get(i,j);
      if (ref == 'x') xCount++;
      if (ref == 'o') oCount++;
    }
  }
  return xCount == oCount;
}

int countBlanks(const Board& board){
  int count = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      const char& ref = board.get(i,j);
      if (ref == '_') count++;
    }
  }
  return count;
}

int countSymbols(const Board& board){
  int count = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      const char& ref = board.get(i,j);
      if (ref == 'x' || ref == 'o') count++;
    }
  }
  return count;
}

char getSymbol(int& x){
  switch (x%3)
  {
    case 0: return '_';
    case 1: return 'x';
    case 2: return 'o';
  }
  return 'n';
}

int numStates = 0;
void createBoards(std::vector<Board>& boards) {
  int combinations = pow(3,numCells);
  for(int i = 0; i < combinations; i++)
  {
      char potential[N][N];
      int combination = i;
      for (int squareNum = 0; squareNum < numCells; squareNum++)
      {
        int row = squareNum / N;
        int col = squareNum % N;
        potential[row][col] = getSymbol(combination);
        combination /= 3;
      }
      if(isValidTTTBoard(potential)) boards.push_back(potential);
  }
}

bool decision(Board board){
  return countBlanks(board) >= 12;
}

class TTTSolver : public Memoizable<Move, Board> {
 public:
  Move solve(Board board) override {
    bool xTurn = isXTurn(board);

    int score = evaluate(board.board);
    if (score == 10) return Move{0,0,1};
    if (score == -10) return Move{0,0,-1};
    if (isMovesLeft(board.board) == false) return Move{0,0,0};
    
    Move best;
    bool firstAssign = true;
    char play = xTurn ? 'x' : 'o';
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (board.get(i,j) == '_') {
          board.assign(i,j,play);
          Move attempt = solve(board);
          if(firstAssign){
            best = attempt;
            firstAssign = false;
          }
          else if(best.isOtherBetter(attempt,xTurn)) best = attempt;
          board.assign(i,j,'_');
        }
      }
    }
    return best;
  }
};

string tttKey(Board board) {
  return string(&board.board[0][0], &board.board[2][2] + 1);
}

void runTTT(TTTSolver& solver, std::vector<Board>& input,
              string path, bool cppersist) {
  Timer timer;
  timer.start();
  for (std::vector<Board>::iterator it = input.begin(); it != input.end(); it++) {
    Move answer = solver.solve(*it);
  }
  timer.end();
  appendRowToFile(path, timer.getRow());
}

void runTTT(std::vector<Board>& input, string type,
              bool cppersist, bool recursive, bool keepCache) {
  string path = getOutPath("TTTSize"+to_string(N), type, cppersist, recursive, keepCache);
  if (recursive) {
    TTTSolver rec;
    auto localMemo = getLocalMemoizedObj<TTTSolver>(
        tttKey, Move::toString, Move::fromString, identity<string>);
    localMemo.setDecision(decision);
    if (!cppersist) {
      runTTT(rec, input, path, cppersist);
    } else {
      runTTT(localMemo, input, path, cppersist);
    }
  }
}

void runTTTWORep(std::vector<Board>& input, bool cppersist,
                   bool recursive, bool keepCache, int seed) {
  shuffle(input.begin(), input.end(), default_random_engine(seed));
  runTTT(input, "WORep", cppersist, recursive, keepCache);
}

void runTTTWRep(std::vector<Board>& input, bool cppersist,
                  bool recursive, bool keepCache, int seed, int numInp) {
  srand(seed);
  std::vector<Board> newInp;
  while (newInp.size() != numInp) {
    newInp.push_back(input[rand() % input.size()]);
  }
  runTTT(newInp, "WRep", cppersist, recursive, keepCache);
}


int main(int argc, char const* argv[]) {
  //43046721 
  std::vector<Board> input;
  createBoards(input);
  int numInput = stoi(argv[1]);
  bool cppersist = stoi(argv[2]);
  bool recursive = stoi(argv[3]);
  bool keepCache = stoi(argv[4]);
  int seed = stoi(argv[5]);
  const char* version = argv[6];

  if (std::strcmp(version, "worep") == 0) {
    runTTTWORep(input, cppersist, recursive, keepCache, seed);
  }

  if (std::strcmp(version, "wrep") == 0) {
    runTTTWRep(input, cppersist, recursive, keepCache, seed, numInput);
  }
  return 0;
}
