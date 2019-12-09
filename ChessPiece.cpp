#include "ChessPiece.h"
#include <iostream>
using namespace std;

ChessPiece::ChessPiece(string player, string type) : player(player), type(type) {};

void ChessPiece::getVerticalPath (int sRank, int dRank, int file, struct move_info info) {
  *info.stepCount = dRank > sRank ? dRank - sRank : sRank - dRank;
  int i = 0;
  for(int rank = sRank; i < *info.stepCount; dRank > sRank ? rank++ : rank--) {
    if (rank != sRank) {
      info.rankSteps[i] = rank;
      info.fileSteps[i] = file;
      ++i;
    }
  }
};

void ChessPiece::getHorizontalPath (int sFile, int dFile, int rank, struct move_info info) {
  *info.stepCount = dFile > sFile ? dFile - sFile : sFile - dFile;
  int i = 0;
  for(int file = sFile; i < *info.stepCount; dFile > sFile ? file++ : file--) {
    if (file != sFile) {  
      info.fileSteps[i] = file;
      info.rankSteps[i] = rank;
      ++i;
    }
  }
};

void ChessPiece::getDiagonalPath(int sRank, int dRank, int sFile, int dFile, struct move_info info) {
  int i = 0;
  for (int rank = sRank; i < *info.stepCount; dRank > sRank ? rank++ : rank--) {
    if (rank != sRank)
      info.rankSteps[i++] = rank;  
  }
  int j = 0;
  for (int file = sFile; j < *info.stepCount; dFile > sFile ? file++ : file--) {
    if (file != sFile)
      info.fileSteps[j++] = file;  
  }
};

Pawn::Pawn(string player) : ChessPiece(player, PAWN) {};

bool Pawn::rules(const char * src, const char * dest, struct move_info info, bool capture) {
  int sRank, sFile, dRank, dFile;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);
  int & stepCount = *info.stepCount;
  stepCount = player == WHITE ? dRank - sRank : sRank - dRank;
  
  if (capture) {
    // Pawn must move 1 step diagonally to capture an opponent's piece 
    if (stepCount != 1 || (dFile - sFile != 1 && dFile - sFile != -1))
      return false;
    return true;
  } 

  if (player == WHITE) {
    // Pawn can only move forward vertically if it's not capturing 
    if (dFile != sFile || dRank <= sRank)
      return false;
    // Pawn can move one or two steps from its initial position 
    if (!moved) {
      if (stepCount > 2)
	      return false;
      if (stepCount == 2) {
        // Assign to rankSteps and fileSteps the index of steps btw src and dest
        info.rankSteps[0] = sRank + 1;
        info.fileSteps[0] = sFile;
      }
    } else {
      // Pawn can only move one step forward after its first move 
      if (stepCount != 1)
	      return false;
    }
  } else {
    // same rules apply to Black pawn 
    if (dFile != sFile || dRank >= sRank)
      return false;
    if (!moved) {
      if (stepCount > 2)
	      return false;
      if (stepCount == 2) {
        getVerticalPath (sRank, dRank, dFile, info);
        // info.rankSteps[0] = sRank - 1;
        // info.fileSteps[0] = sFile;
      }
    } else {
      if (stepCount != 1)
	      return false;
    }
  }
  return true;
};

Rook::Rook(string player) : ChessPiece(player, ROOK) {};

bool Rook::rules(const char * src, const char * dest, struct move_info info, bool capture) {
  int sRank, sFile, dRank, dFile;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);
  // Rook moves vertical- / horizontally
  if (dFile != sFile && dRank != sRank)
    return false;
  if (dFile == sFile) {
    getVerticalPath (sRank, dRank, dFile, info);
  } else if (dRank == sRank) {
    getHorizontalPath (sFile, dFile, dRank, info);
  }
  return true;
};

Knight::Knight(string player) : ChessPiece(player, KNIGHT) {};

bool Knight::rules(const char * src, const char * dest, struct move_info info, bool capture) {
  int sRank, sFile, dRank, dFile;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);

  // Knight moves to the closest square that are not on the same rank / file
  if (dFile == sFile || dRank == sRank)
    return false;

  int const rankDistance = dRank > sRank ? dRank - sRank : sRank - dRank;
  int const fileDistance = dFile > sFile ? dFile - sFile : sFile - dFile;

  // Knight moves to the closest square that is not on its diagonal
  if (rankDistance == fileDistance || rankDistance > 2 || fileDistance > 2)
    return false;

  // Knight can leap over pieces - no need to check steps between src and dest
  
  return true;
};

Bishop::Bishop(string player) : ChessPiece(player, BISHOP) {};

bool Bishop::rules(const char * src, const char * dest, struct move_info info, bool capture) {
  int sRank, sFile, dRank, dFile;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);
  
  // Bishop moves diagonally
  if (dFile == sFile || dRank == sRank)
    return false;

  int const rankDistance = dRank > sRank ? dRank - sRank : sRank - dRank;
  int const fileDistance = dFile > sFile ? dFile - sFile : sFile - dFile;

  // The horizontal and vertical distance must be equal for diagonal movement
  if (rankDistance != fileDistance)
    return false;
 
  *info.stepCount = rankDistance;
  getDiagonalPath(sRank, dRank, sFile, dFile, info);

  return true;
};

Queen::Queen(string player) : ChessPiece(player, QUEEN) {};

bool Queen::rules(const char * src, const char * dest, struct move_info info, bool capture) {
  int sRank, sFile, dRank, dFile;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);
  
  if (dFile == sFile) {
    getVerticalPath(sRank, dRank, dFile, info);
  } else if (dRank == sRank) {
    getHorizontalPath(sFile, dFile, dRank, info);
  } else {
    int const rankDistance = dRank > sRank ? dRank - sRank : sRank - dRank;
    int const fileDistance = dFile > sFile ? dFile - sFile : sFile - dFile;
    // The horizontal and vertical distance must be equal for diagonal movement
    if (rankDistance != fileDistance)
      return false;

    *info.stepCount = rankDistance;
    getDiagonalPath(sRank, dRank, sFile, dFile, info);
  }
  return true;
};

King::King(string player) : ChessPiece(player, KING) {};

bool King::rules(const char * src, const char * dest, struct move_info info, bool capture) {
  int sRank, sFile, dRank, dFile;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);
  int & stepCount = *info.stepCount;
  
  // King can only move 1 step in any direction
  if (dFile == sFile) {
    // Vertical move
    stepCount = dRank > sRank ? dRank - sRank : sRank - dRank;
    if (stepCount != 1)
      return false;
  } else if (dRank == sRank) {
    // Horizontal move
    stepCount = dFile > sFile ? dFile - sFile : sFile - dFile;
    // Castling - king can move 2 squares horizontally from its initial position
    if (stepCount == 2 && !moved && !capture) {
      // Set data member 'castling' to true
      castling = true;
      getHorizontalPath(sFile, dFile, dRank, info);
      return true;
    }
    if (stepCount != 1)
      return false;    
  } else {
    // Diagonal move
    int const rankDistance = dRank > sRank ? dRank - sRank : sRank - dRank;
    int const fileDistance = dFile > sFile ? dFile - sFile : sFile - dFile;

    if (fileDistance != 1 || rankDistance != 1)
      return false;
    
    stepCount = rankDistance;
  }
  return true;
};


