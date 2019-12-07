#include "ChessPiece.h"

class ChessPiece;

class ChessBoard {
 private:
  /*
   * A 2-d array (8 x 8) of ChessPiece * initialised to nullptr
   * Each pointer represents a square of the chessboard 
   * */
  ChessPiece * board[MAX_RANGE][MAX_RANGE] = {};

  /* 
   * Incremented on each player's move
   * Used to keep track of the player's turn
   * */
  int moveCount = 0;

  /* 
   * This function sets up the initial position of pieces of a player
   * Parameter: player - "White" / "Black"
   * */
  void setPieces(string player);

  /* 
   * This function checks:
   * the range of the source (src) coordinates
   * whether there is a piece at the source square 
   * whether it is the player's turn to move
   * Parameter: src, player 
   * */
  bool isValidSource(const char * src, std::string player);

  /* 
   * This function checks:
   * the range of the destination (dest) coordinates
   * whether the destination square is empty
   * if dest not empty, whether the square is occupied by the opponent
   * if occupied by opponent, this function sets the parameter 'capture' to true
   * */
  bool isValidDestination(const char * dest, std::string player, bool& capture);
  
  /* 
   * This function checks:
   * whether the path between src and dest is clear
   * if the move will lead to a check - in which case the move is illegal
   * if the moving piece is a King attempting castling, whether it is legal
   * */
  bool isValidMove(const char * src, const char * dest, bool capture, bool& castling, std::string player);

  bool isPathClear(const char * src, const char * dest, bool capture = false);

  /* 
   * If dest is occupied, 'capture' the piece - delete the ChessPiece instance
   * Move the piece on the src square to the dest square
   * Increment moveCount
   * */
  void makeMove(const char * src, const char * dest, bool castling);
  
  void simulateMove(const char * src, const char * dest, ChessPiece * sim_board[][MAX_RANGE]);

  bool isInCheck(std::string player, ChessPiece * cb[][MAX_RANGE]);

  bool playerHasPossibleMoves(std::string player);

  bool pieceHasPossibleMoves(const char * piece);

  void getKingPosition(std::string player, int& kingRank, int&kingFile, ChessPiece * cb[][MAX_RANGE]);

  bool isValidCastling(const char * kingPosition, const char * dest);

  /* 
   * This function cleans up 'board' 
   * All existing ChessPiece objects are deleted here
   * */
  void cleanUp();

 public:
  /* 
   * ChessBoard constructor 
   * resetBoard function is called here
   * */
  ChessBoard();
  
  /* 
   * This function resets the chessboard 
   * Each player's pieces are set to their initial positions
   * moveCount is set to 0
   * */
  void resetBoard();
  
  void submitMove(const char * src, const char * dest);

  ~ChessBoard();
};
