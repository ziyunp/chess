#include <string>
using namespace std;

int const MAX_RANGE = 8;
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

  int moveCount = 0;

  bool isValidSource(const char * src, string currPlayer);
  
  bool isValidDestination(const char * dest, string currPlayer, bool& capture);

  void makeMove(const char * src, const char * dest);
  
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

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture);
};

class Pawn : public ChessPiece {
  
 public:
  
  Pawn(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture);

};

class Rook : public ChessPiece {
  
 public:

  Rook(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture);

};

class Knight : public ChessPiece {
  
 public:
  
  Knight(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture);
 
};

class Bishop : public ChessPiece {
  
 public:
  
  Bishop(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture);
 
};

class Queen : public ChessPiece {
  
 public:
  
  Queen(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture);
 
};

class King : public ChessPiece {
  
 public:
  
  King(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture);
 
};

void getIndex(const char * position, int& rank, int& dest);
