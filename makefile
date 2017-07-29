c: a.out
	./a.out
a.out: main.cpp Chess.h Chess.h.gch
	g++ main.cpp
Chess.h.gch: Chess.h Board.cpp Piece.cpp Move.cpp ChessUtility.cpp
	g++ Chess.h