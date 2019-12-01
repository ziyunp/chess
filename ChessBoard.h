#include <string>
using namespace std;

int const MAX_RANGE = 8;
int const MAX_KING_RANGE = 3;
string const WHITE = "White";
string const BLACK = "Black";
string const PAWN = "Pawn";
string const ROOK = "Rook";
string const KNIGHT = "Knight";
string const BISHOP = "Bishop";
string const QUEEN = "Queen";
string const KING = "King";

class ChessBoard;
class ChessPiece;

class ChessBoard {
  ChessPiece* board[MAX_RANGE][MAX_RANGE] = {};

  void setPieces(string player);
  /* Used to keep track of the player's turn - even number = White's turn, odd = Black's turn */
  int moveCount = 0;
  /* 
   * This function checks:
   * the range of the src coordinates,
   * whether there is a piece at the source square, 
   * whether it is the player's turn to move
   * */
  bool isValidSource(const char * src, string currPlayer);
  /* 
   * This function checks:
   * the range of the dest coordinates,
   * whether the destination square is empty, 
   * if dest not empty, whether the piece belongs to the opponent. 
   * If the dest is occupied by an opponent piece, this function sets the   parameter 'capture' to true
   * */
  bool isValidDestination(const char * dest, string currPlayer, bool& capture);
  
  /* 
   * If dest is occupied, 'capture' the piece - delete the ChessPiece instance
   * Move the piece on the src square to the dest square
   * Increment moveCount
   * */
  void makeMove(const char * src, const char * dest);
  
  void simulateMove(const char * src, const char * dest, ChessPiece * sim_board[][MAX_RANGE]);

  bool isInCheck(string player, ChessPiece * cb[][MAX_RANGE]);

  bool isCheckmate(string player);

  bool isStalemate(string player);

  void getKingPosition(string player, int& kingRank, int&kingFile);

 public:
  ChessBoard ();
  
  void resetBoard();
  
  void submitMove(const char * src, const char * dest);
  
};

class ChessPiece{

 public:
  string player;
  string type;
  ChessPiece(string player, string const type);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture = false) = 0;
};

class Pawn : public ChessPiece {
  
 public:
  
  Pawn(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture = false);

};

class Rook : public ChessPiece {
  
 public:

  Rook(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture = false);

};

class Knight : public ChessPiece {
  
 public:
  
  Knight(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture = false);
 
};

class Bishop : public ChessPiece {
  
 public:
  
  Bishop(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture = false);
 
};

class Queen : public ChessPiece {
  
 public:
  
  Queen(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture = false);
 
};

class King : public ChessPiece {
  
 public:
  
  King(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture = false);
 
};

void getIndex(const char * position, int& rank, int& dest);

char * getCoord(int const rank, int const file);