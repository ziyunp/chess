#include <string>
#include "utils.h"

/* 
 * Stores the path taken by the piece to move from src to dest
 * Each coord (index of rank & file) along the path is stored in rankSteps and fileSteps respectively
 * */
struct move_info {
  int * rankSteps = {};
  int * fileSteps = {};
  int * stepCount = {};
};

class ChessPiece{
 protected:
  /* 
   * These functions get the path btw the source(s-) and destination(d-) index:
   * assign to rankSteps and fileSteps(in move_info) the index of each step
   * assign number of steps btw src and dest to stepCount(in move_info)
   * */
  void getVerticalPath(int sRank, int dRank, int file, struct move_info info);
  void getHorizontalPath(int sFile, int dFile, int rank, struct move_info info);
  void getDiagonalPath(int sRank, int dRank, int sFile, int dFile, struct move_info info);

 public:
  // "White" / "Black"
  std::string player;
  // "Pawn" / "Rook" / "Knight"...
  std::string type;
  // Set to true once the piece is moved
  bool moved = false;

  /* 
   * ChessPiece constructor
   * initialises data members 'player' & 'type'
   * */
  ChessPiece(std::string player, std::string type);

  /* 
   * Pure virtual function for rules to move the chesspiece. Each derived class has different rules
   * This function checks if the destination is within the range where the piece can move
   * info - stepCount and coordinates of each square along the path are added to the struct to be checked by ChessBoard functions
   * Parameters: src, dest, info (carries information about the move), capture (different rules may apply if the piece is capturing)
   * */
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
  /* 
   * Set to true if the king attempts to castle 
   * provided it has not moved & the destination square is empty
   * */
  bool castling = false;

  King(std::string player);

  virtual bool rules(const char * src, const char * dest, struct move_info info, bool capture = false);
};
