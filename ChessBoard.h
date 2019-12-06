#include "ChessPiece.h"
using namespace std;

class ChessPiece;

class ChessBoard {
  ChessPiece * board[MAX_RANGE][MAX_RANGE] = {};

  /* Used to keep track of the player's turn - even number = White's turn, odd = Black's turn */
  int moveCount = 0;

  void setPieces(string player);

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
  
  bool isPathClear(const char * src, const char * dest, bool capture = false);

  bool isValidMove(const char * src, const char * dest, bool capture, bool& castling, string player);

  /* 
   * If dest is occupied, 'capture' the piece - delete the ChessPiece instance
   * Move the piece on the src square to the dest square
   * Increment moveCount
   * */
  void makeMove(const char * src, const char * dest, bool castling);
  
  void simulateMove(const char * src, const char * dest, ChessPiece * sim_board[][MAX_RANGE]);

  bool isInCheck(string player, ChessPiece * cb[][MAX_RANGE]);

  bool playerHasPossibleMoves(string player);

  bool pieceHasPossibleMoves(const char * piece);

  void getKingPosition(string player, int& kingRank, int&kingFile, ChessPiece * cb[][MAX_RANGE]);

  bool isValidCastling(const char * kingPosition, const char * dest);

  void cleanUp();

 public:

  ChessBoard();
  
  void resetBoard();
  
  void submitMove(const char * src, const char * dest);

  ~ChessBoard();
};
