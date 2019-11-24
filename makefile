chess: ChessMain.o ChessBoard.o
	g++ ChessMain.o ChessBoard.o -o chess

ChessMain.o: ChessMain.cpp ChessBoard.h
	g++ -g -Wall -c ChessMain.cpp

ChessBoard.o: ChessBoard.cpp ChessBoard.h
	g++ -g -Wall -c ChessBoard.cpp
