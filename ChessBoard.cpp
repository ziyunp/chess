#include <iostream>
#include "ChessBoard.h"

using namespace std;

ChessBoard::ChessBoard() {
  resetBoard();
}

void ChessBoard::resetBoard() {  
  cout << "A new chess game is started!\n";

  moveCount = 0;
  
  cleanUp();

  setPieces(WHITE);
  setPieces(BLACK);
};

void ChessBoard::setPieces(string player) {
  int rank, file;
  
  // set pawns
  rank = player == WHITE ? 1 : 6;
  for (file = 0; file < 8; file++) {
    board[rank][file] = new Pawn(player);
  }
  // set other pieces
  rank = player == WHITE ? 0 : 7;
  
  board[rank][0] = new Rook(player);
  board[rank][1] = new Knight(player);
  board[rank][2] = new Bishop(player);
  board[rank][3] = new Queen(player);
  board[rank][4] = new King(player);
  board[rank][5] = new Bishop(player);
  board[rank][6] = new Knight(player);
  board[rank][7] = new Rook(player);
};

void ChessBoard::submitMove(const char * src, const char * dest) {
  // Checking which player's turn - White's turn when moveCount is even 
  string currPlayer = moveCount % 2 ? BLACK : WHITE;

  bool capture = false, castling = false;

  // Check whether src and dest are valid
  // isValidDestination may modify capture's value - useful info for isValidMove
  if(isValidSource(src, currPlayer) && isValidDestination(dest, currPlayer, capture)) {    
    
    // isValidMove may modify castling's value - useful info for makeMove
    if(isValidMove(src, dest, capture, castling, currPlayer)) {
      
      // Move the piece from src to dest if all the checks are passed
      makeMove(src, dest, castling);

      // Check if the opponent's king is in check
      string oppPlayer = currPlayer == WHITE ? BLACK : WHITE;
      if (isInCheck(oppPlayer, board)) {
        // Check if the opponent is in checkmate
        if(!playerHasPossibleMoves(oppPlayer)) {
          cout << oppPlayer << " is in checkmate\n";
          return;
        };
        cout << oppPlayer << " is in check\n";
      } else {
        // If not in check, check if the game is in stalemate
        if (!playerHasPossibleMoves(oppPlayer)) {
          cout << "The game is in stalemate!\n";
          return;
        }
      }
    } else {
      int rank, file;
      getIndex(src, rank, file);
      cout << currPlayer << "'s " << board[rank][file]->type << " cannot move to " << dest << "!\n";
    }
  }
};

bool ChessBoard::isValidSource(const char * src, string player) {
  int sRank, sFile; 
  getIndex(src, sRank, sFile);
  ChessPiece * piece = board[sRank][sFile];
  if (sRank < 0 || sRank > 7 || sFile < 0 || sFile > 7) {
    cout << "Source square " << src << " out of range. Rank must be between 1-8 and file must be between A-H\n";
    return false;
  }
  
  if(piece == NULL) {
    cout << "There is no piece at position " << src << "!\n";
    return false;
  }

  if(piece->player != player) {
    cout << "It is not " << piece->player << "'s turn to move!\n";
    return false;
  }
  return true;
};

bool ChessBoard::isValidDestination(const char * dest, string player, bool& capture) {
  int dRank, dFile;
  getIndex(dest, dRank, dFile);
  ChessPiece * piece = board[dRank][dFile];
  if (dRank < 0 || dRank > 7 || dFile < 0 || dFile > 7) {
    cout << "Destination square " << dest << " out of range. Rank must be between 1-8 and file must be between A-H\n";
    return false;
  }
  // Destination square is empty
  if(piece == NULL)
    return true;

  // Destination square is occupied by the opponent
  if(piece->player != player) {
    capture = true;
    return true;
  }
  cout << "The destination " << dest << " is occupied by " << piece->player << "'s " << board[dRank][dFile]->type << endl;
  return false;
};

bool ChessBoard::isValidMove(const char * src, const char * dest, bool capture, bool& castling, string player) {
  // Check if the path taken by the piece to move from src to dest is clear
  if (isPathClear(src, dest, capture, board)) {
    int sRank, sFile;
    getIndex(src, sRank, sFile);
    ChessPiece * piece = board[sRank][sFile];
    // If the pice is a king that is castling, check if castling is valid
    if (piece->type == KING) {
      King * king_ptr = static_cast<King*> (piece);
      if (king_ptr->castling) {
        // Reset 'castling' (King's data member)
        king_ptr->castling = false;
        if(isValidCastling(src, dest)) {
          castling = true;
          return true;
        }
        return false;
      }
    }
    // Simulate move and check if this move will put own's king in check
    ChessPiece * sim_board[MAX_RANGE][MAX_RANGE];
    simulateMove(src, dest, sim_board);
    if (isInCheck(player, sim_board))
      return false;

    // Valid move if path is clear and will not lead to a check
    return true;
  }
  return false;
}

bool ChessBoard::isPathClear(const char * src, const char * dest, bool capture, ChessPiece * chessboard[][MAX_RANGE]) {
  int sRank, sFile;
  getIndex(src, sRank, sFile);
  
  int rankSteps[MAX_RANGE], fileSteps[MAX_RANGE], stepCount = 0;

  move_info moveInfo;
  moveInfo.rankSteps = &rankSteps[0];
  moveInfo.fileSteps = &fileSteps[0];
  moveInfo.stepCount = &stepCount;

  // Checks the piece's rules to see whether it can move from src to dest 
  if(chessboard[sRank][sFile]->rules(src, dest, moveInfo, capture)) {
    // Checks if the path is blocked by any pieces
    for (int i = 0; i < stepCount - 1; i++) {
      if (chessboard[rankSteps[i]][fileSteps[i]] != NULL) {
        return false;
      }
    }
    return true;
  } 
  return false;
}

bool ChessBoard::isValidCastling(const char * kingPosition, const char * dest) {
  int kingRank, kingFile, dRank, dFile;
  getIndex(kingPosition, kingRank, kingFile);
  getIndex(dest, dRank, dFile);
  ChessPiece * king = board[kingRank][kingFile];
  string player = king->player;

  // Rule: King cannot castle out of check
  if (isInCheck(player, board))
    return false;

  // Determine Queenside / Kingside rook 
  ChessPiece * rook = dFile < kingFile ? board[dRank][0] : board[dRank][7];
  
  // Rule: Rook must not have been moved 
  if (rook->moved)
    return false;

  // Rule: King cannot castle through / into check
  int file = dFile < kingFile ? kingFile - 1 : kingFile + 1;
  for (; dFile < kingFile ? file >= dFile : file <= dFile; dFile < kingFile ? file-- : file++) {
    // Simulate moving king to each square and check whether is in check
    char * currSquare = getCoord(kingRank, file);
    ChessPiece * sim_board[MAX_RANGE][MAX_RANGE];
    simulateMove(kingPosition, currSquare, sim_board); 
    delete currSquare;

    if (isInCheck(player, sim_board))
      return false;
  }
  return true;
}

void ChessBoard::simulateMove(const char * src, const char * dest, ChessPiece * sim_board[][MAX_RANGE]) {
  int sRank, sFile, dRank, dFile;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);

  // Copy the 'real' board into the sim_board (simulated board)
  for (int rank = 0; rank < MAX_RANGE; rank++) {
    for (int file = 0; file < MAX_RANGE; file++) {
      sim_board[rank][file] = board[rank][file];
    }
  }
  // Make move on the sim_board 
  sim_board[dRank][dFile] = sim_board[sRank][sFile];
  sim_board[sRank][sFile] = NULL;
};

bool ChessBoard::isInCheck(string player, ChessPiece * chessboard[][MAX_RANGE]) {
  char * kingPosition = getKingPosition(player, chessboard);
  bool capture = true;

  // Loop through every square of the chessboard to find all opponent's pieces
  for (int rank = 0; rank < MAX_RANGE; rank++) {
    for (int file = 0; file < MAX_RANGE; file++) {
      ChessPiece * piece = chessboard[rank][file];
      // For each opponent piece, check if the piece is checking the king 
      if(piece != NULL && piece->player != player) {
        char * piecePosition = getCoord(rank, file);
        // Check if the piece can move towards the king 
        if (isPathClear(piecePosition, kingPosition, capture, chessboard)) {
          delete piecePosition;
          delete kingPosition;
          return true;
        }
        delete piecePosition;
      }
    }
  }
  delete kingPosition;
  return false;
};


void ChessBoard::makeMove(const char * src, const char * dest, bool castling) {
  int sRank, sFile, dRank, dFile;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);
  ChessPiece * movingPiece = board[sRank][sFile];

  // Set 'moved' to true as a record that this piece has moved
  if (!movingPiece->moved)
    movingPiece->moved = true;

  // Stores the information about the captured piece for an informative output
  string capturedPlayer = "", capturedPiece = "";
  // 'Capture' the piece on the destination square  
  if (board[dRank][dFile] != NULL) {
    capturedPlayer = board[dRank][dFile]->player;
    capturedPiece = board[dRank][dFile]->type;
    delete board[dRank][dFile];
    board[dRank][dFile] = NULL;
  }
  // 'Move' the piece from src to dest
  board[dRank][dFile] = movingPiece;
  board[sRank][sFile] = NULL;
  
  // If 'castling' is taking place, move the rook after the king moved
  if (castling) {
    // Determine whether the Queenside rook or Kingside rook is moving
    int rookFile = dFile < sFile ? 0 : 7;
    ChessPiece * rook = board[dRank][rookFile];
    // Set 'moved' to true as a record that this piece has moved
    rook->moved = true;
    // Determine which file the rook is castling to
    int cFile = dFile < sFile ? 3 : 5;
    // Move the rook
    board[dRank][cFile] = rook;
    board[dRank][rookFile] = NULL;
    // Gets the src and dest coordinates of the rook for an informative output
    char * rookSrc = getCoord(dRank, rookFile);
    char * rookDest = getCoord(dRank, cFile);
    
    cout << movingPiece->player << "'s " << movingPiece->type << " castles from " << src << " to " << dest << endl;
    cout << rook->player << "'s " << rook->type << " castles from " << rookSrc << " to " << rookDest;

    delete rookSrc;
    delete rookDest;
  } else {
    cout << movingPiece->player << "'s " << movingPiece->type << " moves from " << src << " to " << dest;
    
    if (capturedPlayer != "" && capturedPiece != "")
      cout << " taking " << capturedPlayer << "'s " << capturedPiece;
  }
  cout << endl;
  moveCount++;
};

bool ChessBoard::playerHasPossibleMoves(string player) {
  // Loop through every square of the chessboard to find all the player's pieces
  for (int rank = 0; rank < MAX_RANGE; rank++) {
    for (int file = 0; file < MAX_RANGE; file++) {
      ChessPiece * piece = board[rank][file];
      // For each player's piece, check if it has any possible moves
      if (piece != NULL && piece->player == player) {
        char * piecePosition = getCoord(rank, file);
        if (pieceHasPossibleMoves(piecePosition)) {
          delete piecePosition;
          return true;
        }
        delete piecePosition;
      }
    }
  }
  // Looped through all pieces and none has possible moves
  return false;
};

bool ChessBoard::pieceHasPossibleMoves(const char * piecePosition) {
  int pieceRank, pieceFile;
  getIndex(piecePosition, pieceRank, pieceFile);
  string player = board[pieceRank][pieceFile]->player;
  // Loop through every square to search for possible moves
  // TODO: maybe reduce the scope of search with moveRange
  for (int rank = 0; rank < MAX_RANGE; rank++) {
    for (int file = 0; file < MAX_RANGE; file++) {
      ChessPiece * piece = board[rank][file];
      // Possible to move to an empty square or to capture an opponent
      if(piece == NULL || piece->player != player) {
        char * currSquare = getCoord(rank, file);
        bool castling, capture = piece == NULL ? false : true;
        // Check if moving to this square is valid
        if(isValidMove(piecePosition, currSquare, capture, castling, player)) {
          delete currSquare;
          return true;
        }
        delete currSquare;
      }
    }
  }
  // Looped through all squares without finding a valid move for the piece 
  return false;
};

char * ChessBoard::getKingPosition(string player, ChessPiece * chessboard[][MAX_RANGE]) {
  int rank = player == WHITE ? 0 : 7;
  for (; player == WHITE ? rank < MAX_RANGE : rank >= 0; player == WHITE ? rank++ : rank--) {
    for (int file = 0; file < MAX_RANGE; file++) {
      ChessPiece * piece = chessboard[rank][file];
      if (piece != NULL && piece->type == KING && piece->player == player) {
        return getCoord(rank, file);
      }
    }
  }
  return NULL;
};

void ChessBoard::cleanUp() {
  for (int rank = 0; rank < MAX_RANGE; rank++) {
    for (int file = 0; file < MAX_RANGE; file++) {
      if(board[rank][file] != NULL) {
        delete board[rank][file];
        board[rank][file] = NULL;
      }
    }
  }
}

ChessBoard::~ChessBoard() {
  cleanUp();
}
