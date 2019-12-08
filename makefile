chess: ChessMain.o ChessBoard.o ChessPiece.o utils.o
	g++ ChessMain.o ChessBoard.o ChessPiece.o utils.o -o chess

ChessMain.o: ChessMain.cpp ChessBoard.h
	g++ -g -Wall -c ChessMain.cpp

ChessBoard.o: ChessBoard.cpp ChessBoard.h ChessPiece.h
	g++ -g -Wall -c ChessBoard.cpp

ChessPiece.o: ChessPiece.cpp ChessPiece.h utils.h
	g++ -g -Wall -c ChessPiece.cpp

utils.o: utils.cpp utils.h
	g++ -g -Wall -c utils.cpp

clean:
	rm -f *.o chess