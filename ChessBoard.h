#include <string>
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
   * Used to keep track of which player's turn
   * */
  int moveCount = 0;

  /* 
   * This function sets up the initial position of pieces of a player
   * Parameter: player - "White" / "Black"
   * */
  void setPieces(string player);

  /* 
   * This function checks:
   * the range of the source coordinates (src) 
   * whether there is a piece at the source square 
   * whether it is the player's turn to move
   * Parameters: src, player 
   * */
  bool isValidSource(const char * src, std::string player);

  /* 
   * This function checks:
   * the range of the destination coordinates (dest) 
   * whether the destination square is empty or is occupied by the opponent
   * if occupied by opponent, this function sets the parameter 'capture' to true
   * Parameters: dest, player, capture (info to be passed to the caller)
   * */
  bool isValidDestination(const char * dest, std::string player, bool& capture);
  
  /* 
   * This function checks:
   * whether the path between src and dest is clear
   * if the move will lead to a check - in which case the move is illegal
   * if the moving piece is a King attempting castling, whether it is legal
   * Parameters: src, dest, capture (indicates if the move involves capturing), castling (info to be passed to the caller), player
   * */
  bool isValidMove(const char * src, const char * dest, bool capture, bool& castling, std::string player);

  /* 
   * This function checks:
   * if the piece's rules are satisfied to move from src to dest
   * whether the path between src and dest is not blocked by any pieces
   * Parameters: 
   * src, dest, chessboard (where the move is made), capture (indicates if the move involves capturing)
   * */
  bool isPathClear(const char * src, const char * dest,  bool capture, ChessPiece * chessboard[][MAX_RANGE]);
  
  /* 
   * This function checks the rules of castling 
   * Parameters: kingPosition, dest
   * */
  bool isValidCastling(const char * kingPosition, const char * dest);
  
  /* 
   * This function simulates moving a piece from src to dest
   * A copy of the real board is made onto sim_board
   * Simulation of the move is made on sim_board
   * Parameters: src, dest, sim_board 
   * */
  void simulateMove(const char * src, const char * dest, ChessPiece * sim_board[][MAX_RANGE]);

  /* 
   * This function checks whether the player's king is in check 
   * Parameters: player, chessboard (the board to be checked)
   * */
  bool isInCheck(std::string player, ChessPiece * chessboard[][MAX_RANGE]);
  
  /* 
   * This function moves the piece on the src square to the dest square
   * Set moved (ChessPiece's data member) to true if this is the 1st move
   * Increment moveCount (ChessBoard's data member)
   * If dest is occupied, 'capture' the piece - delete the ChessPiece instance
   * If castling is true, move both the king and the rook
   * Parameters: src, dest, castling (indicates if this move is castling)
   * */
  void makeMove(const char * src, const char * dest, bool castling);

  /* 
   * This function checks if the player still has possible moves on the 'board'
   * Parameter: player
   * */
  bool playerHasPossibleMoves(std::string player);

  /* 
   * This function checks if the piece still has possible moves on the 'board'
   * Parameter: piece
   * */
  bool pieceHasPossibleMoves(const char * piece);

  /* 
   * This function searches the player's king on the chessboard 
   * Returns the king's coordinates
   * Parameters: player, chessboard 
   * */
  char * getKingPosition(std::string player, ChessPiece * chessboard[][MAX_RANGE]);

  /* 
   * This function cleans up the 'board' 
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
  
  /*
   * This function allows the user to submit moves between two squares
   * If the move is valid, a piece on src square will be moved to dest square
   * Parameters: src - source / initial coordinates of the moving piece 
   *             dest - coordinates destination
   * */
  void submitMove(const char * src, const char * dest);
  
  /* 
   * ChessBoard destructor 
   * cleanUp function is called here
   * */
  ~ChessBoard();
};
