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

bool isMovesLeft(char board[3][3]) {
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      if (board[i][j] == '_') return true;
  return false;
}

int evaluate(char b[3][3]) {
  // Checking for Rows for X or O victory.
  for (int row = 0; row < 3; row++) {
    if (b[row][0] == b[row][1] && b[row][1] == b[row][2]) {
      if (b[row][0] == player)
        return +10;
      else if (b[row][0] == opponent)
        return -10;
    }
  }

  // Checking for Columns for X or O victory.
  for (int col = 0; col < 3; col++) {
    if (b[0][col] == b[1][col] && b[1][col] == b[2][col]) {
      if (b[0][col] == player)
        return +10;

      else if (b[0][col] == opponent)
        return -10;
    }
  }

  // Checking for Diagonals for X or O victory.
  if (b[0][0] == b[1][1] && b[1][1] == b[2][2]) {
    if (b[0][0] == player)
      return +10;
    else if (b[0][0] == opponent)
      return -10;
  }

  if (b[0][2] == b[1][1] && b[1][1] == b[2][0]) {
    if (b[0][2] == player)
      return +10;
    else if (b[0][2] == opponent)
      return -10;
  }

  // Else if none of them have won then return 0
  return 0;
}




int count(char curBoard[3][3], bool xs) {
  int xCount = 0;
  int oCount = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (curBoard[i][j] == 'x') xCount++;
      if (curBoard[i][j] == 'o') oCount++;
    }
  }
  return xs ? xCount : oCount;
}

bool isValidTTTBoard(char curBoard[3][3]) {
  int xCount = count(curBoard, true);
  int oCount = count(curBoard, false);
  return ( (xCount == oCount) || (oCount == (xCount-1)) ) 
            && (evaluate(curBoard) == 0) && (xCount != 5);
}
void copyBoardTo(char from[3][3], char to[3][3]) {
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) to[i][j] = from[i][j];
}

class Board {
 public:
  char board[3][3];
  Board(char board[3][3]) { copyBoardTo(board, this->board); }
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
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      const char& ref = board.get(i,j);
      if (ref == 'x') xCount++;
      if (ref == 'o') oCount++;
    }
  }
  return xCount == oCount;
}

int numStates = 0;
void createBoards(int square_num, char curBoard[3][3],
                  std::vector<Board>& boards, int& index) {
  if (square_num == 9) {
    if (isValidTTTBoard(curBoard)) {
      Board toAdd(curBoard);
      boards.push_back(toAdd);
      // boards[index] = toAdd;
      // index++;
      numStates++;
    }
    return;
  }

  int row = square_num / 3;
  int col = square_num % 3;
  for (int i = 0; i <= 2; i++) {
    if (i == 0) curBoard[row][col] = '_';
    if (i == 1) curBoard[row][col] = 'x';
    if (i == 2) curBoard[row][col] = 'o';
    createBoards(square_num + 1, curBoard, boards, index);
  }
}

class TTTSolver : public PersistentMemoizable<Move, Board> {
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
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
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
  string path = getOutPath("TTT", type, cppersist, recursive, keepCache);
  if (recursive) {
    TTTSolver rec;
    auto localMemo = getLocalMemoizedObj<TTTSolver>(
        tttKey, Move::toString, Move::fromString, identity<string>);
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
                  bool recursive, bool keepCache, int seed) {
  srand(seed);
  std::vector<Board> newInp;
  int ind = 0;
  while (newInp.size() != input.size()) {
    newInp.push_back(input[rand() % input.size()]);
  }
  runTTT(newInp, "WRep", cppersist, recursive, keepCache);
}


int main(int argc, char const* argv[]) {

  int numInput = stoi(argv[1]);
  bool cppersist = stoi(argv[2]);
  bool recursive = stoi(argv[3]);
  bool keepCache = stoi(argv[4]);
  int seed = stoi(argv[5]);
  const char* version = argv[6];

  char empty[3][3] = {{'_', '_', '_'}, {'_', '_', '_'}, {'_', '_', '_'}};
  std::vector<Board> input;
  int index = 0;
  createBoards(0, empty, input, index);
  if (std::strcmp(version, "worep") == 0) {
    runTTTWORep(input, cppersist, recursive, keepCache, seed);
  }

  if (std::strcmp(version, "wrep") == 0) {
    runTTTWRep(input, cppersist, recursive, keepCache, seed);
  }
  return 0;
}
