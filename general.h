using namespace std;

#define MAX_RANGE 8

#define WHITE "White"
#define BLACK "Black"
#define PAWN "Pawn"
#define ROOK "Rook"
#define KNIGHT "Knight"
#define BISHOP "Bishop"
#define QUEEN "Queen"
#define KING "King"

#define FILE_A 'A'
#define RANK_1 '1'

void getIndex(const char * position, int& rank, int& dest);

char * getCoord(int const rank, int const file);
