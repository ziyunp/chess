#include <iostream>
#include "ChessPiece.h"
#include "general.h"
using namespace std;

// void ChessPiece::getAllPossibleMoves(const char * src) {
//   char * dest = getCoord(MAX_RANGE, MAX_RANGE);
//   int rankSteps[MAX_RANGE];
//   int fileSteps[MAX_RANGE];
//   int stepCount = 0;
//   this->rules(src, dest, rankSteps, fileSteps, stepCount);

// };  
ChessPiece::ChessPiece(string player, string const type) : player(player), type(type) {};

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

// void King::getAllPossibleMoves(const char * src, int possibleRank[], int possibleFile[]) {
//   int sRank, sFile;
//   getIndex(src, sRank, sFile);
//   int r = 0, f = 0;
//   /* King can move one square from its position, hence: -1, +0, +1 */
//   for (int i = 0; i < moveRange; i++) {
//     /* Ensure that the square is not out of range */
//     if (sRank - (i - 1) >= 0 || sRank - (i - 1) < MAX_RANGE) 
//       possibleRank[r++] = sRank - (i - 1);
//     if (sFile - (i - 1) >= 0 || sFile - (i - 1) < MAX_RANGE) 
//       possibleFile[f++] = sFile - (i - 1);
//   };
//   int possibleMoveCount = r * f;
// };
