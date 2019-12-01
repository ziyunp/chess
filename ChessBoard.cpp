#include <iostream>
#include <cstring>
#include "ChessBoard.h"
using namespace std;

ChessBoard::ChessBoard() {
  resetBoard();
}

void ChessBoard::resetBoard() {
  int rank, file;
  
  cout << "A new chess game is started!\n";
  
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

  bool captureOpponent = false;

  if(isValidSource(src, currPlayer) && isValidDestination(dest, currPlayer, captureOpponent)) {
    int sRank, sFile;
    getIndex(src, sRank, sFile);
    
    int rankSteps[MAX_RANGE];
    int fileSteps[MAX_RANGE];
    int stepCount = 0;

    /* Check if the squares between src and dest are empty */
    if(board[sRank][sFile]->rules(src, dest, rankSteps, fileSteps, stepCount, captureOpponent)) {
      for (int i = 0; i < stepCount - 1; i++) {
        // cout << "i: " << i << " file rank:" << fileSteps[i] << rankSteps[i] << endl;
        if (board[rankSteps[i]][fileSteps[i]] != NULL) {
          cout << currPlayer << "'s " << board[sRank][sFile]->type << " cannot move to " << dest << "!\n";
          return;
        }
      }
      /* invalid move if this move will put own's king in check */
      /* Simulate move */
      ChessPiece * sim_board[MAX_RANGE][MAX_RANGE];
      simulateMove(src, dest, sim_board);
      if (isInCheck(currPlayer, sim_board)) {
        cout << "Invalid move - this move will put your king in check!\n";
        return;
      }

      /* make move */
      makeMove(src, dest);

      /* Check if the opponent king is in check */
      string oppPlayer = currPlayer == WHITE ? BLACK : WHITE;
      if (isInCheck(oppPlayer, board)) {
        /* Check is the opponent is in checkmate */
      } else {
        /* If not in check, check is the game in stalemate */
        if (isStalemate(oppPlayer)) {
          cout << "Stalemate!\n";
          exit(0);
        }
      }
    } else {
      cout << "Invalid move - " << board[sRank][sFile]->type << " cannot move from " << src << " to " << dest << "!\n";;
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

void ChessBoard::makeMove(const char * src, const char * dest) {
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
  
  board[dRank][dFile] = board[sRank][sFile];
  board[sRank][sFile] = NULL;
  
  cout << board[dRank][dFile]->player << "'s " << board[dRank][dFile]->type << " moves from " << src << " to " << dest;
  
  if (capturedPlayer != "" && capturedPiece != "")
    cout << " taking " << capturedPlayer << "'s " << capturedPiece;

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
  getKingPosition(player, kingRank, kingFile);
  int rankSteps[MAX_RANGE], fileSteps[MAX_RANGE], stepCount = 0;
  bool capture = true;
  char * dest = getCoord(kingRank, kingFile);

  /* Loop through every square of the chessboard*/
  for (rank = 0; rank < MAX_RANGE; rank++) {
    for (file = 0; file < MAX_RANGE; file++) {
      /* For each opponent piece, check if the piece is checking the king */
      if(cb[rank][file] != NULL && cb[rank][file]->player != player) {
        char * src = getCoord(rank, file);
        /* Check if the piece can move towards the king */
        if (cb[rank][file]->rules(src, dest, rankSteps, fileSteps, stepCount, capture)) {
          int i;
          for (i = 0; i < stepCount - 1; i++) {
            if (board[rankSteps[i]][fileSteps[i]] != NULL)
              break;
          }
          /* No pieces blocking between */
          if (i == stepCount - 1 || stepCount <= 1) {
            cout << "in check\n";

            return true; 
          }
        }
      }
    } 
  }
  return false;
};

bool ChessBoard::isCheckmate(string player) {
  /* Can the king move out of check? */
  // simulate move, isInCheck?, one no => return false
  /* Can any pieces capture the piece checking the king, including the king */
  // simulate move, isInCheck?, one no => return false
  /* Can any pieces block the check */
  // need to know the path between checking piece and king
};

bool ChessBoard::isStalemate(string player) {
  int kingRank, kingFile;
  getKingPosition(player, kingRank, kingFile);

  int possibleRank[MAX_KING_RANGE], possibleFile[MAX_KING_RANGE];
  int r = 0, f = 0;
  /* King can move one square from its position, hence: -1, +0, +1 */
  for (int i = 0; i < MAX_KING_RANGE; i++) {
    /* Ensure that the square is not out of range */
    if (kingRank - (i - 1) >= 0 || kingRank - (i - 1) < MAX_RANGE) 
      possibleRank[r++] = kingRank - (i - 1);
    if (kingFile - (i - 1) >= 0 || kingFile - (i - 1) < MAX_RANGE) 
      possibleFile[f++] = kingFile - (i - 1);
  };
  int possibleMoveCount = r * f;
  char * src = getCoord(kingRank, kingFile);
  /* If all possible moves will lead to in check, then it is a stalemate */
  /* Possible moves = all combinations of the possible ranks and files */
  int rank, file;
  for (rank = 0; rank < MAX_KING_RANGE; rank++) {
    for (file = 0; file < MAX_KING_RANGE; file++) {
      /* If the square is not empty, this is not a possible move */
      if (board[possibleRank[rank]][possibleFile[file]] != NULL) {
        possibleMoveCount--;
        continue;
      }
      char * dest = getCoord(possibleRank[rank], possibleFile[file]);
      ChessPiece * sim_board[MAX_RANGE][MAX_RANGE];
      /* Simulate moving king to this position and check if king is in check */
      simulateMove(src, dest, sim_board);
      if (!isInCheck(player, sim_board))
        return false;
    }
  }
  if (possibleMoveCount == 0)
    return false;
  return true;
};

void ChessBoard::getKingPosition(string player, int& kingRank, int&kingFile) {
  int rank, file;
  rank = player == WHITE ? 0 : 7;
  for (; player == WHITE ? rank < MAX_RANGE : rank >= 0; player == WHITE ? rank++ : rank--) {
    for (file = 0; file < MAX_RANGE; file++) {
      if (board[rank][file] != NULL && board[rank][file]->type == KING && board[rank][file]->player == player) {
        kingRank = rank;
        kingFile = file;
        return;
      }
    }
  }
};

ChessPiece::ChessPiece(string player, string const type) : player(player), type(type) {};

bool ChessPiece::rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture) {
  return true;
};

Pawn::Pawn(string player) : ChessPiece(player, PAWN) {};

bool Pawn::rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture) {
  int sRank, sFile, dRank, dFile;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);

  if (player == WHITE) {
    /*Pawn can only move forward*/
    if (dRank <= sRank)
      return false;
    /*Pawn can move one or two steps from its starting position */
    stepCount = dRank - sRank;
    if (sRank == 1) {
      if (stepCount > 2)
	      return false;
      if (stepCount == 2) {
        rankSteps[0] = sRank + 1;
        fileSteps[0] = sFile;
      }
    } else {
      /*Pawn can only move one step forward after its first move */
      if (stepCount != 1)
	      return false;
    }
  } else {
    /* same rules apply to Black pawn */
    if (dRank >= sRank)
      return false;
    stepCount = sRank - dRank;
    if (sRank == 6) {
      if (stepCount > 2)
	      return false;
      if (stepCount == 2) {
        rankSteps[0] = sRank - 1;
        fileSteps[0] = sFile;
      }
    } else {
      if (stepCount != 1)
	      return false;
    }
  }
  if (capture) {
    /*Pawn must move diagonally to capture an opponent's piece */
    if (dFile - sFile != 1 && dFile - sFile != -1)
      return false;
  } else {
    /*Pawn can only move in the same file if it's not capturing */
    if (dFile != sFile)
      return false;
  }
  return true;
};

Rook::Rook(string player) : ChessPiece(player, ROOK) {};

bool Rook::rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture) {
  int sRank, sFile, dRank, dFile;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);

  /* Rook moves vertical- / horizontally */
  if (dFile != sFile && dRank != sRank)
    return false;
  /* vertical move */
  if (dFile == sFile) {
    stepCount = dRank > sRank ? dRank - sRank : sRank - dRank;
    int i = 0;
    for(int rank = sRank; i < stepCount; dRank > sRank ? rank++ : rank--) {
      /* Ignore the source rank */
      if (rank != sRank) {
        rankSteps[i] = rank;
        fileSteps[i++] = dFile;
      }
    }
  } else if (dRank == sRank) {
    stepCount = dFile > sFile ? dFile - sFile : sFile - dFile;
    int i = 0;
    for(int file = sFile; i < stepCount; dFile > sFile ? file++ : file--) {
      /* Ignore the source file */
      if (file != sFile) {
        fileSteps[i] = file;
        rankSteps[i++] = dRank;
      }
    }
  }
  return true;
};

Knight::Knight(string player) : ChessPiece(player, KNIGHT) {};

bool Knight::rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture) {
  int sRank, sFile, dRank, dFile;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);

  /* Knight moves to the closest square that are not on the same rank/file */
  if (dFile == sFile || dRank == sRank)
    return false;

  int const rankDistance = dRank > sRank ? dRank - sRank : sRank - dRank;
  int const fileDistance = dFile > sFile ? dFile - sFile : sFile - dFile;

  /* Knight moves to the closest square that is not on its diagonal */
  if (rankDistance == fileDistance || rankDistance > 2 || fileDistance > 2)
    return false;

  /* Knight can leap over pieces hence no need to check the steps between src and dest */
  
  return true;
};

Bishop::Bishop(string player) : ChessPiece(player, BISHOP) {};

bool Bishop::rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture) {
  int sRank, sFile, dRank, dFile;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);

  /* Bishop moves diagonally */
  if (dFile == sFile || dRank == sRank)
    return false;

  int const rankDistance = dRank > sRank ? dRank - sRank : sRank - dRank;
  int const fileDistance = dFile > sFile ? dFile - sFile : sFile - dFile;

  /* The horizontal and vertical distance must be equal for diagonal movement */
  if (rankDistance != fileDistance)
    return false;

  /* Assign to rankSteps and fileSteps the index of steps btw src and dest */
  int i = 0;
  for (int rank = sRank; i < rankDistance; dRank > sRank ? rank++ : rank--) {
    /* Ignore the source rank */
    if (rank != sRank)
      rankSteps[i++] = rank;  
  }

  int j = 0;
  for (int file = sFile; j < fileDistance; dFile > sFile ? file++ : file--) {
    /* Ignore the source file */
    if (file != sFile)
      fileSteps[j++] = file;  
  }

  stepCount = rankDistance;
  
  return true;
};

Queen::Queen(string player) : ChessPiece(player, QUEEN) {};

bool Queen::rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture) {
  int sRank, sFile, dRank, dFile;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);

  if (dFile == sFile) {
    stepCount = dRank > sRank ? dRank - sRank : sRank - dRank;
    int i = 0;
    for(int rank = sRank; i < stepCount; dRank > sRank ? rank++ : rank--) {
      /* Ignore the source rank */
      if (rank != sRank) {
        rankSteps[i] = rank;
        fileSteps[i++] = dFile;
      }
    }
  } else if (dRank == sRank) {
    stepCount = dFile > sFile ? dFile - sFile : sFile - dFile;
    int i = 0;
    for(int file = sFile; i < stepCount; dFile > sFile ? file++ : file--) {
      /* Ignore the source file */
      if (file != sFile) {
        fileSteps[i] = file;
        rankSteps[i++] = dRank;
      }
    }
  } else {
    int const rankDistance = dRank > sRank ? dRank - sRank : sRank - dRank;
    int const fileDistance = dFile > sFile ? dFile - sFile : sFile - dFile;
    
    /* The horizontal and vertical distance must be equal for diagonal movement */
    if (rankDistance != fileDistance)
      return false;
    
    /* Assign to rankSteps and fileSteps the index of steps btw src and dest */
    int i = 0;
    for (int rank = sRank; i < rankDistance; dRank > sRank ? rank++ : rank--) {
      /* Ignore the source rank */
      if (rank != sRank)
	      rankSteps[i++] = rank;  
    }
    
    int j = 0;
    for (int file = sFile; j < fileDistance; dFile > sFile ? file++ : file--) {
      /* Ignore the source file */
      if (file != sFile)
	      fileSteps[j++] = file;  
    }
    
    stepCount = rankDistance;
  }
  return true;
};

King::King(string player) : ChessPiece(player, KING) {};

bool King::rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture) {
  int sRank, sFile, dRank, dFile;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);

  /* King can only move 1 step in any direction */
  if (dFile == sFile) {
    /* Horizontal move */
    stepCount = dRank > sRank ? dRank - sRank : sRank - dRank;
    if (stepCount != 1)
      return false;
  } else if (dRank == sRank) {
    /* Vertical move */
    stepCount = dFile > sFile ? dFile - sFile : sFile - dFile;
    if (stepCount != 1)
      return false;    
  } else {
    /* Diagonal move */
    int const rankDistance = dRank > sRank ? dRank - sRank : sRank - dRank;
    int const fileDistance = dFile > sFile ? dFile - sFile : sFile - dFile;

    if (fileDistance != 1 || rankDistance != 1)
      return false;
    
    stepCount = rankDistance;
  }

  return true;
};

void getIndex(const char * position, int& rank, int& file) {
  file = position[0] - 'A';
  rank = position[1] - '1'; 
}

char * getCoord(int const rank, int const file) {
  char * position = new char[2];
  position[0] = file + 'A';
  position[1] = rank + '1'; 
  return position;
}


