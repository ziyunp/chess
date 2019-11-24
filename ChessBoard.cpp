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

void ChessBoard::submitMove(const char * src, const char * dest) {
  string currPlayer = moveCount % 2 ? BLACK : WHITE;

  bool captureOpponent = false;

  if(isValidSource(src, currPlayer) && isValidDestination(dest, currPlayer, captureOpponent)) {
    int sRank, sFile;
    getIndex(src, sRank, sFile);
    
    /*If the moving piece is a Knight, no need to check the following*/
    int rankSteps[MAX_RANGE];
    int fileSteps[MAX_RANGE];
    int stepCount = 0;

    /*If rules are satisfied, check if the path is clear*/
    if(board[sRank][sFile]->rules(src, dest, rankSteps, fileSteps, stepCount, captureOpponent)) {
      for (int i = 0; i < stepCount; i++) {
	//	cout << "i: " << i << " file rank:" << fileSteps[i] << rankSteps[i] << endl;
	if (board[rankSteps[i]][fileSteps[i]] != NULL) {
	  cout << currPlayer << "'s " << board[sRank][sFile]->type << " cannot move to " << dest << "!\n";
	  return;
	}
      }
      /* make move */
      makeMove(src, dest);
    }
  }
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
  
  if (board[dRank][dFile] != NULL) {
    cout << "b4 delete\n";
    delete board[dRank][dFile];
    cout << "after delete\n";
    board[dRank][dFile] = NULL;
    cout << "after set null\n";
  }
  
  board[dRank][dFile] = board[sRank][sFile];
  board[sRank][sFile] = NULL;
  
  cout << board[dRank][dFile]->player << "'s " << board[dRank][dFile]->type << " moves from " << src << " to " << dest << endl;

  moveCount++;
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
    if (sRank == 1) {
      if (dRank - sRank > 2)
	return false;
      if (dRank - sRank == 2) {
	rankSteps[0] = sRank + 1;
	fileSteps[0] = sFile;
	stepCount = 1;
      }
    } else {
      /*Pawn can only move one step forward after its first move */
      if (dRank - sRank != 1)
	return false;
    }

    if (capture) {
      /*Pawn must move diagonally to capture an opponent's piece*/
      if (dFile - sFile != 1 && dFile - sFile != -1)
	return false;
    } else {
      /*Pawn can only move in the same file if it's not capturing*/
      if (dFile != sFile)
	return false;
    }
  } else {
    /* same rules apply to Black pawn */
    if (dRank >= sRank)
      return false;
    
    if (sRank == 6) {
      if (sRank - dRank > 2)
	return false;
      if (sRank - dRank == 2) {
	rankSteps[0] = sRank - 1;
	fileSteps[0] = sFile;
	stepCount = 1;
      }
    } else {
      if (sRank - dRank != 1)
	return false;
    }
    
    if (capture) {
      /*Pawn must move diagonally to capture an opponent's piece*/
      if (dFile - sFile != 1 && dFile - sFile != -1)
	return false;
    } else {
      /*Pawn can only move in the same file if it's not capturing*/
      if (dFile != sFile)
	return false;
    }
  }
  return true;
};

Rook::Rook(string player) : ChessPiece(player, ROOK) {};

bool Rook::rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture) {
  return true;
};

Knight::Knight(string player) : ChessPiece(player, KNIGHT) {};

bool Knight::rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture) {
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
  return true;
};

King::King(string player) : ChessPiece(player, KING) {};

bool King::rules(const char * src, const char * dest, int rankSteps[], int fileSteps[], int& stepCount, bool capture) {
  return true;
};

void getIndex(const char * position, int& rank, int& file) {
  file = position[0] - 'A';
  rank = position[1] - '1'; // rank '1' has an index of 0 
}
