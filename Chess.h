/**********************************************************************************
*      Chess.h                                                                    *
*                                                                                 *
*                                                                                 *
*      This program is free software; you can redistribute it and/or modify       *
*      it under the terms of the GNU General Public License as published by       *
*      the Free Software Foundation; either version 2 of the License, or          *
*      (at your option) any later version.                                        *
*                                                                                 *
*      This program is distributed in the hope that it will be useful,            *
*      but WITHOUT ANY WARRANTY; without even the implied warranty of             *
*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
*      GNU General Public License for more details.                               *
*                                                                                 *
*      You should have received a copy of the GNU General Public License          *
*      along with this program; if not, see <http://www.gnu.org/licenses/>.       *
*                                                                                 *
***********************************************************************************/

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
    static inline Piece make_piece(const int, const int, const int, const char, Board*);
    static inline Piece make_piece(const int, const int, const char, Board*);
    static inline Piece make_piece(Board*);
    static inline int getColour (const char piece) {return (piece == '.' ? 2 : (piece == 'q' || piece == 'r' || piece == 'b' || piece == 'n' || piece == 'k' || piece == 'p' ? 1 : 0));}
    static inline char sameColour(const char basePiece, const char piece) {return getColour(basePiece) == getColour(piece) ? piece : oppositePiece[piece];}
    friend ostream& operator<<(ostream& os, const Piece& thisPiece) 
    {
#ifdef __unix__
        return os<<pieceDisplay[thisPiece.getPiece()];
#else
        return os<<thisPiece.getPiece();
#endif 
    }
    inline int getColour () const;
    inline pair<int,int> getPos() const;
    inline char getPiece () const;
    inline void setColour (const int);
    inline void setPos (const int, const int);
    inline void setPiece (const char);
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
    void displayBoard(const bool) const;
    bool displayPieceMoves(const char);
    void getPieceMoves(const char);
    bool undoMove();
    void Save_To_File(const string);
    bool Load_From_File(const string, bool &);

private:
    void initialize();
    vector<pair<int,int>> returnPos(char) const;
    void applyMove(Move &);
    bool stalemate(const bool);
    int checkmate(const bool);
    void updateBoard();
    vector<Move> recordedMoves, currentOptions;
};

#include "Move.cpp"
#include "Piece.cpp"
#include "Board.cpp"

#endif