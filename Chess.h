#ifndef CHESS_H
#define CHESS_H

#include "ChessUtility.cpp"

struct Move
{
    bool castle = false;
    int startX = 0, startY = 0, endX = 0, endY = 0,
        rookStartX = 0, rookStartY = 0, rookEndX = 0, rookEndY = 0, pawnSpecial = 0; //0 = nothing, 1 = en passant, 2 = promotion
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
    static Piece make_piece(const int, const int, const int, const char, Board*);
    static Piece make_piece(const int, const int, const char, Board*);
    static Piece make_piece(Board*);
    static inline int getColour (const char piece) {return (piece == '.' ? 2 : (piece == 'q' || piece == 'r' || piece == 'b' || piece == 'n' || piece == 'k' || piece == 'p' ? 1 : 0));}
    static inline char sameColour(const char basePiece, const char piece) {return getColour(basePiece) == getColour(piece) ? piece : oppositePiece[piece];}
    friend ostream& operator<<(ostream& os, const Piece& thisPiece) {return os<<pieceDisplay[thisPiece.getPiece()];}
    int getColour () const;
    pair<int,int> getPos() const;
    char getPiece () const;
    void setColour (const int);
    void setPos (const int, const int);
    void setPiece (const char);
    void getMoves (vector<Move> &);

private:
    Piece(const int, const int, const int, const char, Board*);
    int isMovable();
    vector<pair<int,int>> inCheck(const char);
    Board* thisBoard = nullptr;
    int colour;
    pair<int,int> pos;
    char type;
};

class Board
{
    friend class Piece;
public:
    Piece board[8][8];
    bitset<8> moved[8];
    Board();
    Board(char [8][8]);
    pair<int,bool> nextTurn(bool &);
    void displayBoard(const bool);
    bool displayPieceMoves(const char);
    void getPieceMoves(const char);
    bool undoMove();
    void Save_To_File(const string);
    bool Load_From_File(const string, bool &);

private:
    vector<pair<int,int>> returnPos(char);
    void applyMove(Move &);
    bool stalemate(const bool);
    bool repetition();
    int checkmate(const bool);
    void updateBoard();
    vector<Move> recordedMoves;
    vector<Move> currentOptions;
};

#include "Move.cpp"
#include "Piece.cpp"
#include "Board.cpp"

#endif