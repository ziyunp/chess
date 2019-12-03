#include "general.h"

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
