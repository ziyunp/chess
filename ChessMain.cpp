#include <iostream>
#include "ChessBoard.h"
using namespace std;

int main() {

	cout << "========================\n";
	cout << "Testing the Chess Engine\n";
	cout << "========================\n\n";

	ChessBoard cb;
	cout << '\n';
	cb.submitMove("E2", "E3");
	cout << '\n';

	cb.submitMove("A7", "A5");
	cout << '\n';

	cb.submitMove("E3", "E5");
	cout << '\n';

	cb.submitMove("A8", "A6");
	cout << '\n';

	cb.submitMove("H5", "A5");
	cout << '\n';

	cb.submitMove("H7", "H5");
	cout << '\n';

	cb.submitMove("E2", "E3");
	cout << '\n';

	cb.submitMove("A7", "A5");
	cout << '\n';

	cb.submitMove("D1", "H5");
	cout << '\n';

	cb.submitMove("A8", "A6");
	cout << '\n';

	cb.submitMove("H5", "A5");
	cout << '\n';

	cb.submitMove("H7", "H5");
	cout << '\n';

	cb.submitMove("H2", "H4");
	cout << '\n';

	cb.submitMove("A6", "H6");
	cout << '\n';

	cb.submitMove("A5", "C7");
	cout << '\n';

	cb.submitMove("F7", "F6");
	cout << '\n';

	cb.submitMove("C7", "D7");
	cout << '\n';

	cb.submitMove("E8", "F7");
	cout << '\n';

	cb.submitMove("D7", "B7");
	cout << '\n';

	cb.submitMove("D8", "D3");
	cout << '\n';

	cb.submitMove("B7", "B8");
	cout << '\n';

	cb.submitMove("D3", "H7");
	cout << '\n';

	cb.submitMove("B8", "C8");
	cout << '\n';

	cb.submitMove("F7", "G6");
	cout << '\n';

	cb.submitMove("C8", "E6");
	cout << '\n';

	cout << "=========================\n";
	cout << "Alekhine vs. Vasic (1931)\n";
	cout << "=========================\n\n";

	cb.resetBoard();
	cout << '\n';

	cb.submitMove("E2", "E4");
	cb.submitMove("E7", "E6");
	cout << '\n';

	cb.submitMove("D2", "D4");
	cb.submitMove("D7", "D5");
	cout << '\n';

	cb.submitMove("B1", "C3");
	cb.submitMove("F8", "B4");
	cout << '\n';

	cb.submitMove("F1", "D3");
	cb.submitMove("B4", "C3");
	cout << '\n';

	cb.submitMove("B2", "C3");
	cb.submitMove("H7", "H6");
	cout << '\n';

	cb.submitMove("C1", "A3");
	cb.submitMove("B8", "D7");
	cout << '\n';

	cb.submitMove("D1", "E2");
	cb.submitMove("D5", "E4");
	cout << '\n';

	cb.submitMove("D3", "E4");
	cb.submitMove("G8", "F6");
	cout << '\n';

	cb.submitMove("E4", "D3");
	cb.submitMove("B7", "B6");
	cout << '\n';

	cb.submitMove("E2", "E6");
	cb.submitMove("F7", "E6");
	cout << '\n';

	cb.submitMove("D3", "G6");
	cout << '\n';

cout << "=========================\n";
	cout << "Kasparov vs Behne (2016)\n";
	cout << "=========================\n\n";
	// castling
	cb.resetBoard();
	cout << '\n';
	cb.submitMove("D2", "D4");
	cb.submitMove("D7", "D5");
	cout << '\n';
	cb.submitMove("C1", "F4");
	cb.submitMove("B8", "C6");
	cout << '\n';
	cb.submitMove("E2", "E3");
	cb.submitMove("C8", "F5");
	cout << '\n';
	cb.submitMove("G1", "F3");
	cb.submitMove("E7", "E6");
	cout << '\n';
	cb.submitMove("A2", "A3");
	cb.submitMove("G8", "F6");
	cout << '\n';
	cb.submitMove("C2", "C4");
	cb.submitMove("F8", "E7");
	cout << '\n';
	cb.submitMove("B1", "C3");
	cb.submitMove("E8", "G8"); // castling O-O
	cout << '\n';
	// cb.submitMove("F1", "E2");
	// cb.submitMove("D8", "D6");
	// cout << '\n';
	// cb.submitMove("E1", "G1"); // white castling
	// cb.submitMove("F8", "D8");
	// cout << '\n';
	// cb.submitMove("F1", "E1");
	// cb.submitMove("G8", "F8");
	// cout << '\n';
	// cb.submitMove("G1", "H1");
	// cb.submitMove("F8", "E8");
	// cout << '\n';
	// cb.submitMove("H1", "G1");
	// cb.submitMove("D8", "D7");
	// cout << '\n';
	// cb.submitMove("G1", "H1");
	// cb.submitMove("E8", "C8"); // black castling again
	// cout << '\n';
	cb.submitMove("C4", "D5");
	cb.submitMove("E6", "D5");
	cout << '\n';
	cb.submitMove("F1", "D3");
	cb.submitMove("F5", "G4");
	cout << '\n';
	cb.submitMove("D1", "C2");
	cb.submitMove("G7", "G6");
	cout << '\n';
	cb.submitMove("H2", "H3");
	cb.submitMove("G4", "F5");
	cout << '\n';
	cb.submitMove("D3", "F5");
	cb.submitMove("G6", "F5");  // 12
	cout << '\n';
	cb.submitMove("C2", "F5");
	cb.submitMove("G8", "H8");
	cout << '\n';
	cb.submitMove("F3", "E5");
	cb.submitMove("C6", "E5");
	cout << '\n';
	cb.submitMove("F4", "E5");
	cb.submitMove("F8", "G8");
	cout << '\n';
	cb.submitMove("G2", "G4");
	cb.submitMove("H7", "H6");
	cout << '\n';
	cb.submitMove("H3", "H4");
	cb.submitMove("G8", "G6");
	cout << '\n';
	cb.submitMove("G4", "G5");
	cb.submitMove("H6", "H5"); // 18
	cout << '\n';
	cb.submitMove("C3", "D5");
	cb.submitMove("H8", "G7");
	cout << '\n';
	cb.submitMove("D5", "E7");
	cb.submitMove("D8", "E7");
	cout << '\n';
	cb.submitMove("G5", "F6");
	cb.submitMove("G6", "F6");
	cout << '\n';
	cb.submitMove("H1", "G1"); 
	cb.submitMove("G7", "F8"); // 22
	cout << '\n';
	cb.submitMove("E5", "F6");
	cb.submitMove("E7", "D6");
	cout << '\n';
	cb.submitMove("F6", "E5");
	cb.submitMove("D6", "D5");
	cout << '\n';
	cb.submitMove("E5", "G7");
	cb.submitMove("F8", "E8");
	cout << '\n';
	cb.submitMove("F5", "D5");
	cb.submitMove("A8", "D8"); // 26
	cout << '\n';
	cb.submitMove("D5", "F5");
	cb.submitMove("B7", "B6");
	cout << '\n';
	cb.submitMove("G7", "F6");
	cb.submitMove("D8", "D6");
	cout << '\n';
	cb.submitMove("G1", "G8");
	cout << '\n';
	
	return 0;
}
