#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

#include "../local.hpp"
#include "../utils/files.hpp"
#include "./helpers/Timer.cpp"
typedef unsigned long long largestUnsigned;
using namespace std::chrono;
using namespace std;
using namespace cpst;
const int N = 4;  // The dimension of the TTT board. This implementation only
                  // works for N<=9.
const int numCells = N * N;
const int invalidNumXs =
    (numCells % 2 == 0) ? (numCells / 2) : ((numCells / 2) + 1);

// A move in a tic-tac-toe board
struct Move {
  int row,
      col = 0;  // of the cell that the current player should place their symbol
  int score;    // 1 = x's will win, -1 o's will win, 0 will end in a tie
  // serialize
  static string toString(Move move) {
    std::ostringstream oss;
    oss << move.row << move.col << move.score;
    return oss.str();
  }
  // deserialize
  static Move fromString(string moveStr) {
    return Move{moveStr.at(0) - '0', moveStr.at(1) - '0',
                std::stoi(moveStr.substr(2))};
  }
  // Compare two moves to see what is better for the current player. isX
  // determines the current player (true = x's, false = o's)
  bool isOtherBetter(const Move& other, bool isX) {
    if (isX) {
      return other.score > this->score;
    }
    return other.score < this->score;
  }
};

char player = 'x', opponent = 'o';

// is there any unfilled cells
bool isMovesLeft(char board[N][N]) {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      if (board[i][j] == '_') return true;
  return false;
}

// determines if there is a winner in either a row or column. If row=true, check
// for rows, otherwise check for columns.
int checkRowCol(char b[N][N], bool row) {
  for (int i = 0; i < N; i++) {
    bool allSame = true;
    char prev = row ? b[i][0] : b[0][i];
    for (int j = 1; j < N; j++) {
      char cur = row ? b[i][j] : b[j][i];
      if (prev != cur || cur == '_') {
        allSame = false;
        break;
      }
    }
    if (allSame) {
      if (prev == player)
        return +10;
      else if (prev == opponent)
        return -10;
    }
  }
  return 0;
}

// determine if there's a winner in one diagonal
int checkDiag(char b[N][N]) {
  char prev = b[0][0];
  for (int i = 1; i < N; i++) {
    char cur = b[i][i];
    if (prev != cur || cur == '_') return 0;
  }
  if (prev == player)
    return +10;
  else if (prev == opponent)
    return -10;
  return 0;
}

// determine if there's a winner in the other diagonal
int checkOtherDiag(char b[N][N]) {
  char prev = b[0][N - 1];
  for (int i = 1; i < N; i++) {
    char cur = b[i][N - (i + 1)];
    if (prev != cur || cur == '_') return 0;
  }
  if (prev == player)
    return +10;
  else if (prev == opponent)
    return -10;
  return 0;
}

// determines if either player has won in the board. Returns 10 if x's won, -10
// if o's won, 0 for a tie/no-winner
int evaluate(char b[N][N]) {
  int rows = checkRowCol(b, true);
  if (rows != 0) return rows;

  int cols = checkRowCol(b, false);
  if (cols != 0) return cols;

  int diag1 = checkDiag(b);
  if (diag1 != 0) return diag1;

  int diag2 = checkOtherDiag(b);
  if (diag2 != 0) return diag2;

  return 0;
}

// either count the number of x's or o's filled in a board - determined by 'xs'
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

// checks if a board is valid - game hasn't ended and there's a valid number of
// x's + o's
bool isValidTTTBoard(char curBoard[N][N]) {
  int xCount = count(curBoard, true);
  int oCount = count(curBoard, false);
  return ((xCount == oCount) || (oCount == (xCount - 1))) &&
         (evaluate(curBoard) == 0) && (xCount != invalidNumXs);
}

// copy one board's symbols to another board
void copyBoardTo(char from[N][N], char to[N][N]) {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) to[i][j] = from[i][j];
}

// representing a board
class Board {
 public:
  char board[N][N];  // the symbols
  Board(char board[N][N]) { copyBoardTo(board, this->board); }
  Board() {}
  void assign(int i, int j, char c) { board[i][j] = c; }
  const char& get(int i, int j) const { return board[i][j]; }
};

// returns whether or not it's x's turn on the current board
bool isXTurn(const Board& board) {
  int xCount = 0;
  int oCount = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      const char& ref = board.get(i, j);
      if (ref == 'x') xCount++;
      if (ref == 'o') oCount++;
    }
  }
  return xCount == oCount;
}

// returns the number of unfilled cells on the current board
int countBlanks(const Board& board) {
  int count = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      const char& ref = board.get(i, j);
      if (ref == '_') count++;
    }
  }
  return count;
}

// returns the number of filled cells on the current board
int countSymbols(const Board& board) {
  int count = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      const char& ref = board.get(i, j);
      if (ref == 'x' || ref == 'o') count++;
    }
  }
  return count;
}

// Used to generate boards
char getSymbol(int& x) {
  switch (x % 3) {
    case 0:
      return '_';
    case 1:
      return 'x';
    case 2:
      return 'o';
  }
  return 'n';
}

// Brute force - generate 3^(N*N) boards. This is a much more efficient +
// smarter way than recursively generating boards.
void createBoards(std::vector<Board>& boards) {
  int combinations = pow(3, numCells);  // 3^(N*N) boards
  for (int i = 0; i < combinations; i++) {
    char potential[N][N];
    int combination = i;
    for (int squareNum = 0; squareNum < numCells; squareNum++) {
      int row = squareNum / N;
      int col = squareNum % N;
      potential[row][col] = getSymbol(combination);
      combination /= 3;
    }
    if (isValidTTTBoard(potential)) {
      Board board(potential);
      boards.push_back(board);
    }
  }
}

// Memoization decision
bool decision(Board board) {
  return countBlanks(board) >=
         10;  // there has to be at least 10 blanks on the board
}

class TTTSolver : public Memoizable<Move, Board> {
 public:
  Move solve(Board board) override {  // recursive minimax function - returns
                                      // best move for the current board
    bool xTurn = isXTurn(board);  // check who's turn it is

    int score = evaluate(board.board);        // see if anyone has won
    if (score == 10) return Move{0, 0, 1};    // x's won
    if (score == -10) return Move{0, 0, -1};  // o's won
    if (isMovesLeft(board.board) == false) return Move{0, 0, 0};  // tie

    Move best;  // find the best move to make
    bool firstAssign = true;
    char play = xTurn ? 'x' : 'o';  // the current player's symbol
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (board.get(i, j) == '_') {  // iterate over unfilled characters
          board.assign(i, j,
                       play);  // attempt to play the current player's symbol
          Move attempt = solve(board);  // recurse minimax function
          if (firstAssign) {
            best = attempt;  // this is the first attempted move
            firstAssign = false;
          } else if (best.isOtherBetter(attempt, xTurn))
            best = attempt;  // found a move that is better than the current
                             // best move
          board.assign(i, j, '_');  // backtrack
        }
      }
    }
    return best;  // return the best move
  }
};

string tttKey(Board board) {
  return string(&board.board[0][0],
                &board.board[(N - 1)][(N - 1)] +
                    1);  // key - simply convert the 2D board to a 1D string
}

void runTTT(TTTSolver& solver, std::vector<Board>& input, string path,
            bool cppersist) {
  Timer timer;
  timer.start();
  for (std::vector<Board>::iterator it = input.begin(); it != input.end();
       it++) {
    Move answer = solver.solve(*it);
  }
  timer.end();
  appendRowToFile(path, timer.getRow());
}

void runTTT(std::vector<Board>& input, string type, bool cppersist,
            bool recursive, bool keepCache) {
  string path = getOutPath("TTTSize" + to_string(N), type, cppersist, recursive,
                           keepCache);
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

// simply shuffle all valid boards and make that the workload
void runTTTWORep(std::vector<Board>& input, bool cppersist, bool recursive,
                 bool keepCache, int seed) {
  shuffle(input.begin(), input.end(), default_random_engine(seed));
  runTTT(input, "WORep", cppersist, recursive, keepCache);
}

// randomly select 'numInp' boards from 'input' - with replacement
void runTTTWRep(std::vector<Board>& input, bool cppersist, bool recursive,
                bool keepCache, int seed, int numInp) {
  srand(seed);
  std::vector<Board> newInp;
  while (newInp.size() != numInp) {
    newInp.push_back(input[rand() % input.size()]);
  }
  runTTT(newInp, "WRep", cppersist, recursive, keepCache);
}

int main(int argc, char const* argv[]) {
  std::vector<Board> input;
  createBoards(input);  // enumerate all possible valid boards
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
