#include "constants.h"
using namespace std;

class ChessPiece{

 public:
  string player;
  string type;
  
  ChessPiece(string player, string const type);
  
  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture = false) = 0;

  // virtual void getAllPossibleMoves(const char * src) = 0;
};

class Pawn : public ChessPiece {
  int const moveRange = 1;
  int const maxPossiblePositions = 3;

 public:
  
  Pawn(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture = false);

};

class Rook : public ChessPiece {
  int const moveRange = MAX_RANGE;
  int const maxPossiblePositions = 14;

 public:

  Rook(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture = false);

};

class Knight : public ChessPiece {
  int const moveRange = 5; 
  int const maxPossiblePositions = 8;

 public:
  
  Knight(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture = false);
 
};

class Bishop : public ChessPiece {
  int const moveRange = MAX_RANGE;
  int const maxPossiblePositions = 14;

 public:
  
  Bishop(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture = false);
 
};

class Queen : public ChessPiece {
  int const moveRange = MAX_RANGE;
  int const maxPossiblePositions = 28;

 public:
  
  Queen(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture = false);
 
};

class King : public ChessPiece {
  int const moveRange = 3;
  int const maxPossiblePositions = 8;

 public:
  
  King(string player);

  virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture = false);
 
  // virtual void getAllPossibleMoves(const char * src);
};
