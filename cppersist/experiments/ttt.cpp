// C++ program to find the next optimal move for
// a player
#include <chrono>
#include <algorithm>
#include <iostream>
#include "../local.hpp"
#include "../utils/files.hpp"
typedef unsigned long long largestUnsigned;
using namespace std::chrono;
using namespace std;
using namespace cpst;
#define NUM_TTT_BOARDS 2423
struct Move
{
    int row, col;
    void printMove(){
      cout << "(" << row << "," << col << ")" << endl;
    }
};
 
char player = 'x', opponent = 'o';
 
// This function returns true if there are moves
// remaining on the board. It returns false if
// there are no moves left to play.
bool isMovesLeft(char board[3][3])
{
    for (int i = 0; i<3; i++)
        for (int j = 0; j<3; j++)
            if (board[i][j]=='_')
                return true;
    return false;
}
 
// This is the evaluation function as discussed
// in the previous article ( http://goo.gl/sJgv68 )
int evaluate(char b[3][3])
{
    // Checking for Rows for X or O victory.
    for (int row = 0; row<3; row++)
    {
        if (b[row][0]==b[row][1] &&
            b[row][1]==b[row][2])
        {
            if (b[row][0]==player)
                return +10;
            else if (b[row][0]==opponent)
                return -10;
        }
    }
 
    // Checking for Columns for X or O victory.
    for (int col = 0; col<3; col++)
    {
        if (b[0][col]==b[1][col] &&
            b[1][col]==b[2][col])
        {
            if (b[0][col]==player)
                return +10;
 
            else if (b[0][col]==opponent)
                return -10;
        }
    }
 
    // Checking for Diagonals for X or O victory.
    if (b[0][0]==b[1][1] && b[1][1]==b[2][2])
    {
        if (b[0][0]==player)
            return +10;
        else if (b[0][0]==opponent)
            return -10;
    }
 
    if (b[0][2]==b[1][1] && b[1][1]==b[2][0])
    {
        if (b[0][2]==player)
            return +10;
        else if (b[0][2]==opponent)
            return -10;
    }
 
    // Else if none of them have won then return 0
    return 0;
}
 
// This is the minimax function. It considers all
// the possible ways the game can go and returns
// the value of the board
int minimax(char board[3][3], int depth, bool isMax)
{
    int score = evaluate(board);
 
    // If Maximizer has won the game return his/her
    // evaluated score
    if (score == 10)
        return score;
 
    // If Minimizer has won the game return his/her
    // evaluated score
    if (score == -10)
        return score;
 
    // If there are no more moves and no winner then
    // it is a tie
    if (isMovesLeft(board)==false)
        return 0;
 
    // If this maximizer's move
    if (isMax)
    {
        int best = -1000;
 
        // Traverse all cells
        for (int i = 0; i<3; i++)
        {
            for (int j = 0; j<3; j++)
            {
                // Check if cell is empty
                if (board[i][j]=='_')
                {
                    // Make the move
                    board[i][j] = player;
 
                    // Call minimax recursively and choose
                    // the maximum value
                    best = max( best,
                        minimax(board, depth+1, !isMax) );
 
                    // Undo the move
                    board[i][j] = '_';
                }
            }
        }
        return best;
    }
 
    // If this minimizer's move
    else
    {
        int best = 1000;
 
        // Traverse all cells
        for (int i = 0; i<3; i++)
        {
            for (int j = 0; j<3; j++)
            {
                // Check if cell is empty
                if (board[i][j]=='_')
                {
                    // Make the move
                    board[i][j] = opponent;
 
                    // Call minimax recursively and choose
                    // the minimum value
                    best = min(best,
                           minimax(board, depth+1, !isMax));
 
                    // Undo the move
                    board[i][j] = '_';
                }
            }
        }
        return best;
    }
}
 
// This will return the best possible move for the player
Move findBestMove(char board[3][3])
{
    int bestVal = -1000;
    Move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;
 
    // Traverse all cells, evaluate minimax function for
    // all empty cells. And return the cell with optimal
    // value.
    for (int i = 0; i<3; i++)
    {
        for (int j = 0; j<3; j++)
        {
            // Check if cell is empty
            if (board[i][j]=='_')
            {
                // Make the move
                board[i][j] = player;
 
                // compute evaluation function for this
                // move.
                int moveVal = minimax(board, 0, false);
 
                // Undo the move
                board[i][j] = '_';
 
                // If the value of the current move is
                // more than the best value, then update
                // best/
                if (moveVal > bestVal)
                {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

int count(char curBoard[3][3], bool xs){
  int xCount = 0;
  int oCount = 0;
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      if(curBoard[i][j] == 'x') xCount++;
      if(curBoard[i][j] == 'o') oCount++;
    }
  }
  return xs ? xCount : oCount;
}

bool isValidTTTBoard(char curBoard[3][3]){
  int xCount = count(curBoard,true);
  int oCount = count(curBoard,false);
  return xCount == oCount && (evaluate(curBoard) == 0);
}
void copyBoardTo(char from[3][3], char to[3][3]){
  for(int i=0;i<3;i++) for(int j=0;j<3;j++) to[i][j] = from[i][j];
}

class Board{
  public:
    char board[3][3];
    Board(char board[3][3]){
      copyBoardTo(board,this->board);
    }
    Board(){}
};
int numStates = 0;
void createBoards(int square_num, char curBoard[3][3], Board boards[NUM_TTT_BOARDS], int& index) {
    if (square_num == 9) {
      if(isValidTTTBoard(curBoard)){
        Board toAdd(curBoard);
        boards[index] = toAdd;
        index++;
        numStates++;
      }
      return;
    }

    for (int i = 0; i <= 2; i++) {
      int row = square_num/3;
      int col = square_num%3;
      if(i==0) curBoard[row][col] = '_';
      if(i==1) curBoard[row][col] = 'x';
      if(i==2) curBoard[row][col] = 'o';
      createBoards(square_num+1,curBoard,boards,index);
    }
}





class TTTSolver : public PersistentMemoizable<Move, Board>{
  public:
    Move solve(Board board) override{
      return findBestMove(board.board);
    }
};

string tttKey(Board board){
  return string(&board.board[0][0], &board.board[2][2]+1);
}

string tttPickle(Move move){
  return to_string(move.row) + to_string(move.col);
}

Move tttUnpickle(string move){
  Move ret;
  ret.row = move[0] - 'a';
  ret.col = move[1] - 'a';
  return ret;
}

string tttHash(string key){
  return key;
}


  // char board[3][3] =
  // {
  //     { 'o', 'o', 'x' },
  //     { 'x', '_', 'x' },
  //     { 'x', 'o', 'o' }
  // };

void runTTTSeq(Board boards[NUM_TTT_BOARDS], string outputFile){
  largestUnsigned totalTimeUnmemoized = 0;
  TTTSolver unmemo;
  for(int i=0;i<NUM_TTT_BOARDS;i++){
    auto start = high_resolution_clock::now();
    unmemo.solve(boards[i]);
    auto timeTaken = duration_cast<nanoseconds>(high_resolution_clock::now()-start).count();
    totalTimeUnmemoized += timeTaken;
  }

  auto localMemo = getLocalMemoizedObj<TTTSolver>(tttKey,tttPickle,tttUnpickle,"tttTest",tttHash);
  largestUnsigned totalTimeMemoized = 0;
  for(int i=0;i<NUM_TTT_BOARDS;i++){
    localMemo(boards[i]);
    totalTimeMemoized += localMemo.solveTime;
  }

  string row = to_string(totalTimeUnmemoized) + ", " + to_string(totalTimeMemoized);
  appendRowToFile(outputFile,row);
  // appendRowToFile("./data/TTTSeq.csv",row);
}

#include <vector>

void runTTTWORep(Board boards[NUM_TTT_BOARDS], int seed){
  vector<int> remaining;
  srand(seed);
  for(int i=0;i<NUM_TTT_BOARDS;i++) remaining.push_back(i);
  int index = 0;
  Board newBoardsOrder[NUM_TTT_BOARDS];
  while(!remaining.empty()){
    int randomIndex = rand() % remaining.size();
    Board newBoard(boards[remaining[randomIndex]].board);
    newBoardsOrder[index++] = newBoard;
    remaining.erase(remaining.begin() + randomIndex);
  }
  runTTTSeq(newBoardsOrder,"./data/TTTWORep.csv");
}

void runTTTWRep(Board boards[NUM_TTT_BOARDS], int seed){
  srand(seed);
  int index = 0;
  Board newBoardsOrder[NUM_TTT_BOARDS];
  for(int i=0;i<NUM_TTT_BOARDS;i++){
    Board newBoard(boards[rand() % NUM_TTT_BOARDS].board);
    newBoardsOrder[index++] = newBoard;
  }
  runTTTSeq(newBoardsOrder,"./data/TTTWRep.csv");
}

int main(int argc, char const *argv[])
{
      char empty[3][3] =
    {
        { '_', '_', '_' },
        { '_', '_', '_' },
        { '_', '_', '_' }
    };
    Board boards[NUM_TTT_BOARDS];
    int index = 0;
    createBoards(0,empty,boards,index);
    int input = stoi(argv[1]);
    runTTTWRep(boards,input);
}



  // int emptyCount = 0;
  // for(int board=0;board<NUM_TTT_BOARDS;board++){
  //   bool thisOne = true;
  //   for(int i=0;i<3;i++){
  //     for(int j=0;j<3;j++){
  //       if(newBoardsOrder[board].board[i][j] != '_') thisOne = false;
  //     }
  //   }
  //   if(thisOne){
  //     for(int i=0;i<3;i++){
  //       for(int j=0;j<3;j++) cout << newBoardsOrder[board].board[i][j] << ",";
  //       cout << endl;
  //     }
  //     cout << endl;
  //     emptyCount++;
  //   }
  // }
  // cout << emptyCount << endl;
  // if(emptyCount == 1){
  //   cout << "HAPPY";
  // }