#include <iostream>
#include "ChessPiece.h"
using namespace std;

ChessPiece::ChessPiece(string player, string const type) : player(player), type(type) {};

Pawn::Pawn(string player) : ChessPiece(player, PAWN) {
  //TODO: needed?
  moveRange = 1;
  maxPossiblePositions = 3;

};

bool Pawn::rules(const char * src, const char * dest, struct move_info info, bool capture) {
  int sRank, sFile, dRank, dFile, steps = 0;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);
  int & stepCount = info.stepCount ? *info.stepCount : steps;
  if (player == WHITE) {
    /*Pawn can only move forward*/
    if (dRank <= sRank)
      return false;
    /*Pawn can move one or two steps from its starting position */
    stepCount = dRank - sRank;
    if (!moved) {
      if (stepCount > 2)
	      return false;
      if (stepCount == 2) {
        info.rankSteps[0] = sRank + 1;
        info.fileSteps[0] = sFile;
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
    if (!moved) {
      if (stepCount > 2)
	      return false;
      if (stepCount == 2) {
        info.rankSteps[0] = sRank - 1;
        info.fileSteps[0] = sFile;
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

// ~Pawn::Pawn() {};

Rook::Rook(string player) : ChessPiece(player, ROOK) {
  moveRange = MAX_RANGE;
  maxPossiblePositions = 14;

};

bool Rook::rules(const char * src, const char * dest, struct move_info info, bool capture) {
  int sRank, sFile, dRank, dFile, steps = 0;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);
  int & stepCount = info.stepCount ? *info.stepCount : steps;
  /* Rook moves vertical- / horizontally */
  if (dFile != sFile && dRank != sRank)
    return false;
  if (dFile == sFile) {
    /* Vertical move */
    stepCount = dRank > sRank ? dRank - sRank : sRank - dRank;
    int i = 0;
    for(int rank = sRank; i < stepCount; dRank > sRank ? rank++ : rank--) {
      /* Ignore the source rank */
      if (rank != sRank) {
        info.rankSteps[i] = rank;
        info.fileSteps[i++] = dFile;
      }
    }
  } else if (dRank == sRank) {
    /* Horizontal move */
    stepCount = dFile > sFile ? dFile - sFile : sFile - dFile;
    int i = 0;
    for(int file = sFile; i < stepCount; dFile > sFile ? file++ : file--) {
      /* Ignore the source file */
      if (file != sFile) {
        info.fileSteps[i] = file;
        info.rankSteps[i++] = dRank;
      }
    }
  }
  return true;
};

Knight::Knight(string player) : ChessPiece(player, KNIGHT) {
  moveRange = 5; 
  maxPossiblePositions = 8;

};

bool Knight::rules(const char * src, const char * dest, struct move_info info, bool capture) {
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

Bishop::Bishop(string player) : ChessPiece(player, BISHOP) {
  moveRange = MAX_RANGE;
  maxPossiblePositions = 14;

};

bool Bishop::rules(const char * src, const char * dest, struct move_info info, bool capture) {
  int sRank, sFile, dRank, dFile, steps = 0;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);
  int & stepCount = info.stepCount ? *info.stepCount : steps;
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
      info.rankSteps[i++] = rank;  
  }

  int j = 0;
  for (int file = sFile; j < fileDistance; dFile > sFile ? file++ : file--) {
    /* Ignore the source file */
    if (file != sFile)
      info.fileSteps[j++] = file;  
  }

  stepCount = rankDistance;
  
  return true;
};

Queen::Queen(string player) : ChessPiece(player, QUEEN) {
  moveRange = MAX_RANGE;
  maxPossiblePositions = 28;

};

bool Queen::rules(const char * src, const char * dest, struct move_info info, bool capture) {
  int sRank, sFile, dRank, dFile, steps = 0;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);
  int & stepCount = info.stepCount ? *info.stepCount : steps;
  if (dFile == sFile) {
    stepCount = dRank > sRank ? dRank - sRank : sRank - dRank;
    int i = 0;
    for(int rank = sRank; i < stepCount; dRank > sRank ? rank++ : rank--) {
      /* Ignore the source rank */
      if (rank != sRank) {
        info.rankSteps[i] = rank;
        info.fileSteps[i++] = dFile;
      }
    }
  } else if (dRank == sRank) {
    stepCount = dFile > sFile ? dFile - sFile : sFile - dFile;
    int i = 0;
    for(int file = sFile; i < stepCount; dFile > sFile ? file++ : file--) {
      /* Ignore the source file */
      if (file != sFile) {
        info.fileSteps[i] = file;
        info.rankSteps[i++] = dRank;
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
	      info.rankSteps[i++] = rank;  
    }
    
    int j = 0;
    for (int file = sFile; j < fileDistance; dFile > sFile ? file++ : file--) {
      /* Ignore the source file */
      if (file != sFile)
	      info.fileSteps[j++] = file;  
    }
    
    stepCount = rankDistance;
  }
  return true;
};

King::King(string player) : ChessPiece(player, KING) {
  moveRange = 3;
  maxPossiblePositions = 8;

};

bool King::rules(const char * src, const char * dest, struct move_info info, bool capture) {
  int sRank, sFile, dRank, dFile, steps = 0;
  getIndex(src, sRank, sFile);
  getIndex(dest, dRank, dFile);
  int & stepCount = info.stepCount ? *info.stepCount : steps;
  /* King can only move 1 step in any direction */
  if (dFile == sFile) {
    /* Vertical move */
    stepCount = dRank > sRank ? dRank - sRank : sRank - dRank;
    if (stepCount != 1)
      return false;
  } else if (dRank == sRank) {
    /* Horizontal move */
    stepCount = dFile > sFile ? dFile - sFile : sFile - dFile;
    /* Castling */
    if (stepCount == 2 && !moved && !capture) {
      castling = true;
      int i = 0;
      for(int file = sFile; i < stepCount; dFile > sFile ? file++ : file--) {
        /* Ignore the source file */
        if (file != sFile) {
          info.fileSteps[i] = file;
          info.rankSteps[i++] = dRank;
        }
      }
      return true;
    }
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


