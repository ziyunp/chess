chess: ChessMain.o ChessBoard.o ChessPiece.o general.o
	g++ ChessMain.o ChessBoard.o ChessPiece.o general.o -o chess

ChessMain.o: ChessMain.cpp ChessBoard.h
	g++ -g -Wall -c ChessMain.cpp

ChessBoard.o: ChessBoard.cpp ChessBoard.h ChessPiece.h
	g++ -g -Wall -c ChessBoard.cpp

ChessPiece.o: ChessPiece.cpp ChessPiece.h general.h constants.h
	g++ -g -Wall -c ChessPiece.cpp

general.o: general.cpp general.h
	g++ -g -Wall -c general.cpp

clean:
	rm -f *.o chess