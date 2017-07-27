#ifndef CHESS_H
#define CHESS_H

#include "ChessUtility.cpp"

struct Move
{
    bool castle = false;
    int startX, startY, endX, endY, rookStartX, rookStartY, rookEndX, rookEndY, pawnSpecial = 0; //0 = nothing, 1 = en passant, 2 = promotion
    char takenPiece = '.';

    Move(int, int, int, int, char); //Normal Move
    Move(int, int, int, int, char, int); //Normal Move with special pawn move
    Move(int, int, int, int, int, int, int, int, bool); //Castle move
};

class Board;

class Piece
{
    friend class Board;
public:
    Piece() = default;
    static Piece make_piece(int, int, int, char, Board*);
    static Piece make_piece(int, int, char, Board*);
    static Piece make_piece(Board*);
    void getMoves (vector<Move> &);
    int getColour ();
    void setColour (int);
    pair<int,int> getPos();
    void setPos (int, int);
    char getPiece ();
    void setPiece (char);
    static int getColour (char piece) {return (piece == '.' ? 2 : (piece == 'q' || piece == 'r' || piece == 'b' || piece == 'n' || piece == 'k' || piece == 'p' ? 1 : 0));}

private:
    Piece(int, int, int, char, Board*);
    int isMovable();
    vector<pair<int,int>> inCheck(char);
    void checkedMoves(char, vector<Move> &);
    Board* thisBoard = nullptr;
    int colour;
    pair<int,int> pos;
    char type;
};

class Board
{
public:
    Piece board[8][8];
    bitset<8> moved [8];
    Board();
    void displayBoard(const bool);
    bool stalemate(bool);
    bool repetition();
    int checkmate(char, vector<Move> &);
    void getPieceMoves(char, vector<Move> &);
    bool displayPieceMoves(vector<Move> &);
    void applyMove(Move &);
    void undoMove();
    vector<pair<int,int>> returnPos(char);
    vector<Move> recordedMoves;
private:
    void updateBoard();

};

#include "Move.cpp"
#include "Piece.cpp"
#include "Board.cpp"

#endif