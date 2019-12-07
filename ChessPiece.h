#include "general.h"

struct move_info {
  int * rankSteps = {};
  int * fileSteps = {};
  int * stepCount = {};
};

class ChessPiece{

 public:
  std::string player;
  std::string type;
  bool moved = false;

  // TODO: need these?
  int moveRange = 0;
  int maxPossiblePositions = 0;
  
  ChessPiece(std::string player, std::string const type);
    
  virtual bool rules(const char * src, const char * dest, struct move_info info, bool capture = false) = 0;

  virtual ~ChessPiece() {}; 
};

class Pawn : public ChessPiece {

 public:
  
  Pawn(std::string player);
  
  virtual bool rules(const char * src, const char * dest, struct move_info info, bool capture = false);
};

class Rook : public ChessPiece {

 public:

  Rook(std::string player);

  virtual bool rules(const char * src, const char * dest, struct move_info info, bool capture = false);
};

class Knight : public ChessPiece {

 public:
  
  Knight(std::string player);

  virtual bool rules(const char * src, const char * dest, struct move_info info, bool capture = false);   
};

class Bishop : public ChessPiece {

 public:
  
  Bishop(std::string player);

  virtual bool rules(const char * src, const char * dest, struct move_info info, bool capture = false);  
};

class Queen : public ChessPiece {

 public:
  
  Queen(std::string player);

  virtual bool rules(const char * src, const char * dest, struct move_info info, bool capture = false); 
};

class King : public ChessPiece {

 public:
  bool castling = false;

  King(std::string player);

  virtual bool rules(const char * src, const char * dest, struct move_info info, bool capture = false);
};
