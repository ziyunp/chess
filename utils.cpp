#include "utils.h"
#include <iostream>
using namespace std;
void getIndex(const char * position, int& rank, int& file) {
  if (position[0] == ' ' || position[1] == ' ' || position[2]) {
    throw string("Invalid coordinates. Coordinates must be two-digit value without any spaces.");
  }
  file = position[0] - FILE_A;
  rank = position[1] - RANK_1; 
}

char * getCoord(int const rank, int const file) {
  char * position = new char[3];
  position[0] = file + FILE_A;
  position[1] = rank + RANK_1; 
  position[2] = '\0';
  return position;
}
