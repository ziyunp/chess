#include <iostream>
#include <cstring>
#include "ChessBoard.h"
#include "general.h"
using namespace std;

ChessBoard::ChessBoard() {
  resetBoard();
}

void ChessBoard::resetBoard() {
  int rank, file;
  
  cout << "A new chess game is started!\n";

  moveCount = 0;
  
  for (rank = 0; rank < MAX_RANGE; rank++) {
    for (file = 0; file < MAX_RANGE; file++) {
      if(board[rank][file] != NULL) {
        delete board[rank][file];
        board[rank][file] = NULL;
      }
    }
  }

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
  string currPlayer = moveCount % 2 ? BLACK : WHITE;

  bool captureOpponent = false, castling = false;

  if(isValidSource(src, currPlayer) && isValidDestination(dest, currPlayer, captureOpponent)) {    
    /* Check if the squares between src and dest are empty */
    if(isValidMove(src, dest, captureOpponent, castling, currPlayer)) {
      makeMove(src, dest, castling);

      /* Check if the opponent king is in check */
      string oppPlayer = currPlayer == WHITE ? BLACK : WHITE;
      if (isInCheck(oppPlayer, board)) {
        if(playerHasNoPossibleMoves(oppPlayer)) {
          cout << oppPlayer << " is in checkmate\n";
          return;
        };
        cout << oppPlayer << " is in check\n";
        /* Check is the opponent is in checkmate */
      } else {
        /* If not in check, check is the game in stalemate */
        if (playerHasNoPossibleMoves(oppPlayer)) {
          cout << "Stalemate!\n";
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

bool ChessBoard::isValidSource(const char * src, string currPlayer) {
  int sRank, sFile; 
  getIndex(src, sRank, sFile);

  if (sRank < 0 || sRank > 7 || sFile < 0 || sFile > 7) {
    cout << "Source square " << src << " out of range. Rank must be between 1-8 and file must be between A-H\n";
    return false;
  }
  
  if(board[sRank][sFile] == NULL) {
    cout << "There is no piece at position " << src << "!\n";
    return false;
  }

  if(board[sRank][sFile]->player != currPlayer) {
    cout << "It is not " << board[sRank][sFile]->player << "'s turn to move!\n";
    return false;
  }
  return true;
};

bool ChessBoard::isValidDestination(const char * dest, string currPlayer, bool& capture) {
  int dRank, dFile;
  getIndex(dest, dRank, dFile);

  if (dRank < 0 || dRank > 7 || dFile < 0 || dFile > 7) {
    cout << "Destination square " << dest << " out of range. Rank must be between 1-8 and file must be between A-H\n";
    return false;
  }

  if(board[dRank][dFile] == NULL)
    return true;

  if(board[dRank][dFile]->player != currPlayer) {
    capture = true;
    return true;
  }
  cout << "The destination " << dest << " is occupied by " << board[dRank][dFile]->player << "'s " << board[dRank][dFile]->type << endl;
  return false;
};

void ChessBoard::makeMove(const char * src, const char * dest, bool castling) {
  int sRank, sFile, dRank, dFile;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);

  string capturedPlayer = "", capturedPiece = "";
  /* capture the piece on the destination square */  
  if (board[dRank][dFile] != NULL) {
    capturedPlayer = board[dRank][dFile]->player;
    capturedPiece = board[dRank][dFile]->type;
    delete board[dRank][dFile];
    board[dRank][dFile] = NULL;
  }

  if (!board[sRank][sFile]->moved)
    board[sRank][sFile]->moved = true;
  
  board[dRank][dFile] = board[sRank][sFile];
  board[sRank][sFile] = NULL;

  if (castling) {
    int rookFile = dFile < sFile ? 0 : 7;
    board[dRank][rookFile]->moved = true;
    int cFile = dFile < sFile ? 3 : 5;
    board[dRank][cFile] = board[dRank][rookFile];
    board[dRank][rookFile] = NULL;

    char * rookSrc = getCoord(dRank, rookFile);
    char * rookDest = getCoord(dRank, cFile);

    cout << board[dRank][dFile]->player << "'s " << board[dRank][dFile]->type << " castles from " << src << " to " << dest << endl;
    cout << board[dRank][cFile]->player << "'s " << board[dRank][cFile]->type << " castles from " << rookSrc << " to " << rookDest;

  } else {
    cout << board[dRank][dFile]->player << "'s " << board[dRank][dFile]->type << " moves from " << src << " to " << dest;
    
    if (capturedPlayer != "" && capturedPiece != "")
      cout << " taking " << capturedPlayer << "'s " << capturedPiece;
  }
  cout << endl;

  moveCount++;
};

void ChessBoard::simulateMove(const char * src, const char * dest, ChessPiece * sim_board[][MAX_RANGE]) {
  int sRank, sFile, dRank, dFile;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);

  /* Copy the real board into the simulated board */
  int rank, file;
  for (rank = 0; rank < MAX_RANGE; rank++) {
    for (file = 0; file < MAX_RANGE; file++) {
      sim_board[rank][file] = board[rank][file];
    }
  }
  /* Make move on the sim_board */
  sim_board[dRank][dFile] = sim_board[sRank][sFile];
  sim_board[sRank][sFile] = NULL;
};

bool ChessBoard::isInCheck(string player, ChessPiece * cb[][MAX_RANGE]) {
  int kingRank, kingFile, rank, file;
  getKingPosition(player, kingRank, kingFile, cb);
  char * kingPosition = getCoord(kingRank, kingFile);
  bool capture = true;

  int rankSteps[MAX_RANGE], fileSteps[MAX_RANGE], stepCount = 0;

  move_info moveInfo;
  moveInfo.rankSteps = &rankSteps[0];
  moveInfo.fileSteps = &fileSteps[0];
  moveInfo.stepCount = &stepCount;

  /* Loop through every square of the chessboard*/
  for (rank = 0; rank < MAX_RANGE; rank++) {
    for (file = 0; file < MAX_RANGE; file++) {
      /* For each opponent piece, check if the piece is checking the king */
      if(cb[rank][file] != NULL && cb[rank][file]->player != player) {
        char * piecePosition = getCoord(rank, file);
        /* Check if the piece can move towards the king */
        if (cb[rank][file]->rules(piecePosition, kingPosition, moveInfo, capture)) {
          int i;
          for (i = 0; i < stepCount - 1; i++) {
            if (cb[rankSteps[i]][fileSteps[i]] != NULL)
              break;
          }

          /* No pieces blocking between */
          if (i == stepCount - 1 || stepCount <= 1)             
            return true;
          
        }
      }
    }
  }
  return false;
};

bool ChessBoard::playerHasNoPossibleMoves(string player) {
  /* Loop through all pieces that belong to this player */
  int rank, file;
  for (rank = 0; rank < MAX_RANGE; rank++) {
    for (file = 0; file < MAX_RANGE; file++) {
      if(board[rank][file] != NULL && board[rank][file]->player == player) {
        char * piecePosition = getCoord(rank, file);
        /* If >= 1 piece has possible moves, then it is not in stalemate */
        if (hasPossibleMoves(piecePosition))
          return false;
      }
    }
  }
  /* Looped through all pieces and none has possible moves*/
  return true;
};

bool ChessBoard::hasPossibleMoves(const char * piece) {
  int pieceRank, pieceFile;
  getIndex(piece, pieceRank, pieceFile);
  string player = board[pieceRank][pieceFile]->player;
  int rank, file;
  /* Loop through all squares to search for possible moves */
  // TODO: maybe reduce the scope of search with moveRange
  for (rank = 0; rank < MAX_RANGE; rank++) {
    for (file = 0; file < MAX_RANGE; file++) {
      /* Possible to move to an empty square or to capture an opponent */
      if(board[rank][file] == NULL || board[rank][file]->player != player) {
        char * dest = getCoord(rank, file);
        bool capture = board[rank][file] == NULL ? false : true;
        bool castling = false;
        /* Check if moving to this square is valid */
        if(isValidMove(piece, dest, capture, castling, player))
          return true;
      }
    }
  }
  /* Completed loop without finding a valid move */
  return false;
};

void ChessBoard::getKingPosition(string player, int& kingRank, int&kingFile, ChessPiece * cb[][MAX_RANGE]) {
  int rank, file;
  rank = player == WHITE ? 0 : 7;
  for (; player == WHITE ? rank < MAX_RANGE : rank >= 0; player == WHITE ? rank++ : rank--) {
    for (file = 0; file < MAX_RANGE; file++) {
      if (cb[rank][file] != NULL && cb[rank][file]->type == KING && cb[rank][file]->player == player) {
        kingRank = rank;
        kingFile = file;
        return;
      }
    }
  }
};

bool ChessBoard::isValidMove(const char * src, const char * dest, bool capture, bool& castling, string player) {
  if (isPathClear(src, dest, capture)) {
    int sRank, sFile;
    getIndex(src, sRank, sFile);
    ChessPiece * piece = board[sRank][sFile];
    if (piece->type == KING && piece->castling) {
      piece->castling = false;
      // castling function
      if(isValidCastling(src, dest)) {
        castling = true;
        return true;
      }
      return false;
    }

    /* invalid move if this move will put own's king in check */
    /* Simulate move */
    ChessPiece * sim_board[MAX_RANGE][MAX_RANGE];
    simulateMove(src, dest, sim_board);
    if (isInCheck(player, sim_board))
      return false;
    /* Valid move if path is clear and will not lead to in check */
    return true;
  }
  /* Path is not clear */
  return false;
}

bool ChessBoard::isPathClear(const char * src, const char * dest, bool capture) {
  int sRank, sFile;
  getIndex(src, sRank, sFile);
  int rankSteps[MAX_RANGE], fileSteps[MAX_RANGE], stepCount = 0;

  move_info moveInfo;
  moveInfo.rankSteps = &rankSteps[0];
  moveInfo.fileSteps = &fileSteps[0];
  moveInfo.stepCount = &stepCount;
  /* Check if the squares between src and dest are empty */
  if(board[sRank][sFile]->rules(src, dest, moveInfo, capture)) {
    // cout << "stepcount: " << stepCount << endl;
    for (int i = 0; i < stepCount - 1; i++) {
      // cout << "i: " << i << " file rank:" << fileSteps[i] << rankSteps[i] << endl;
      if (board[rankSteps[i]][fileSteps[i]] != NULL) {
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
  // can only castle when not in check
  if (isInCheck(player, board))
    return false;

  // which rook?
  ChessPiece * rook = dFile < kingFile ? board[dRank][0] : board[dRank][7];
  // rook must not have moved
  if (rook->moved)
    return false;

  // is path being attacked?
  // horizontal move => only check fileSteps
  if (dFile < kingFile) {
    for (int file = kingFile - 1; file >= dFile; file--) {
      char * currSquare = getCoord(kingRank, file);
      // simulate moving king to each square and check whether is in check?
      ChessPiece * sim_board[MAX_RANGE][MAX_RANGE];
      simulateMove(kingPosition, currSquare, sim_board);
      if (isInCheck(player, sim_board))
        return false;
    }
  } else {
    for (int file = kingFile + 1; file <= dFile; file++) {
      char * currSquare = getCoord(kingRank, file);
      // simulate moving king to each square and check whether is in check?
      ChessPiece * sim_board[MAX_RANGE][MAX_RANGE];
      simulateMove(kingPosition, currSquare, sim_board);
      if (isInCheck(player, sim_board))
        return false;
    }
  }
  return true;
}