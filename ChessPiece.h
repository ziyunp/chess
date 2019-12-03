#include "constants.h"
using namespace std;

struct move_info {
  int * rankSteps = {};
  int * fileSteps = {};
  int * stepCount = {};
};

class ChessPiece{

 public:
  string player;
  string type;
  int moveRange = 0;
  int maxPossiblePositions = 0;

  ChessPiece(string player, string const type);
  
  // virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture = false) = 0;
  
  virtual bool rules(const char * src, const char * dest, struct move_info info, bool capture = false) = 0;
};

class Pawn : public ChessPiece {

 public:
  
  Pawn(string player);

  // virtual bool rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture = false);
  
  virtual bool rules(const char * src, const char * dest, struct move_info info, bool capture = false);
};

class Rook : public ChessPiece {

 public:

  Rook(string player);

  virtual bool rules(const char * src, const char * dest, struct move_info info, bool capture = false);

};

class Knight : public ChessPiece {

 public:
  
  Knight(string player);

  virtual bool rules(const char * src, const char * dest, struct move_info info, bool capture = false);
 
};

class Bishop : public ChessPiece {

 public:
  
  Bishop(string player);

  virtual bool rules(const char * src, const char * dest, struct move_info info, bool capture = false);
 
};

class Queen : public ChessPiece {

 public:
  
  Queen(string player);

  virtual bool rules(const char * src, const char * dest, struct move_info info, bool capture = false);
 
};

class King : public ChessPiece {

 public:
  
  King(string player);

  virtual bool rules(const char * src, const char * dest, struct move_info info, bool capture = false);
 
  // virtual void getAllPossibleMoves(const char * src);
};