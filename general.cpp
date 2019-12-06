#include "general.h"

void getIndex(const char * position, int& rank, int& file) {
  file = position[0] - FILE_A;
  rank = position[1] - RANK_1; 
}

char * getCoord(int const rank, int const file) {
  char * position = new char[2];
  position[0] = file + FILE_A;
  position[1] = rank + RANK_1; 
  return position;
}
