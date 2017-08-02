/**********************************************************************************
*      Piece.cpp                                                                  *
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

#include "Chess.h"

//Private Constructor
Piece::Piece(const int colour, const int x, const int y, const char type, Board* location) 
{   
    this->colour = colour, pos = make_pair(x, y), this->type = tolower(type), this->thisBoard = location;
    assert(this->thisBoard != nullptr);
}

//Only way to create a piece
inline Piece Piece::make_piece(const int colour, const int x, const int y, const char type, Board* location) 
    {return Piece(colour, x, y, type, location); }
inline Piece Piece::make_piece(const int x, const int y, const char type, Board* location) 
    {return Piece(2, x, y, type, location); }
inline Piece Piece::make_piece(Board* location) 
    {return Piece(2, 8, 8, '.', location); }

//Public member functions
inline int Piece::getColour () const 
    {return colour; }
inline pair<int,int> Piece::getPos() const
    {return pos; }
inline char Piece::getPiece () const
    {return (getColour(type) == colour ? type : oppositePiece[type]); }

inline void Piece::setColour (const int colour) 
    {this->colour = colour; }
inline void Piece::setPos (const int x, const int y) 
    {pos = make_pair(x, y); }
inline void Piece::setPiece (const char type) 
{
    this->type = tolower(type);
    this->colour = (this->type == '.' ? 2 : Piece::getColour(type));
}

void Piece::getMoves(vector<Move> &validOptions)
{
    int directionsMovable = isMovable(), x = pos.first, y = pos.second;
    switch(tolower(type))
    {
        case 'k':
            //First checks if the king and the rook is on the right x-value (rank) 
            //Then checks if positions between the king's position and the final position is in check
            //Then checks if those positions are empty ('.')
            //Finally checks if both the rook and the king have not moved before
            if ((x == 0 || x == 7) && thisBoard->board[x][y].inCheck(getPiece()).size() == 0 && thisBoard->board[x][y+1].inCheck(getPiece()).size() == 0 && thisBoard->board[x][y+2].inCheck(getPiece()).size() == 0 && thisBoard->board[x][y+1].getPiece() == '.' && thisBoard->board[x][y+2].getPiece() == '.' && !thisBoard->moved[x][7] && !thisBoard->moved[x][y])
                validOptions.push_back(Move(x, y, x, y+2, x, 7, x, y+1, true));
            if ((x == 0 || x == 7) && thisBoard->board[x][y].inCheck(getPiece()).size() == 0 && thisBoard->board[x][y-1].inCheck(getPiece()).size() == 0 && thisBoard->board[x][y-2].inCheck(getPiece()).size() == 0 && thisBoard->board[x][y-1].getPiece() == '.' && thisBoard->board[x][y-2].getPiece() == '.' && !thisBoard->moved[x][0] && !thisBoard->moved[x][y])
                validOptions.push_back(Move(x, y, x, y-2, x, 0, x, y-1, true));
            for (auto& i : range)
                if (inBound(x+QBRKXY[i].first, y+QBRKXY[i].second) && thisBoard->board[x+QBRKXY[i].first][y+QBRKXY[i].second].getColour() != getColour() && thisBoard->board[x+QBRKXY[i].first][y+QBRKXY[i].second].inCheck(getPiece()).size() == 0)
                    validOptions.push_back(Move(x, y, x+QBRKXY[i].first, y+QBRKXY[i].second, thisBoard->board[x+QBRKXY[i].first][y+QBRKXY[i].second].getPiece()));
        break;
        case 'q':
        case 'r':
        case 'b':
            //Queen = 0 - 8, Rook = 0 - 4, Bishop = 4 - 8
            for (unsigned int i = (tolower(type) == 'b' ? 4 : 0); i < (tolower(type) == 'r' ? 4 : 8); i++)
            {
                for (unsigned int j = 1; j < 8; j++)
                {
                    int mX = j*QBRKXY[i].first, mY = j*QBRKXY[i].second;
                    if (!inBound(x+mX, y+mY))
                        break;
                    if (thisBoard->board[x+mX][y+mY].getPiece() != '.')
                    {
                        if (getColour() != thisBoard->board[x+mX][y+mY].getColour() && (directionsMovable == 0 || (directionsMovable == 1 && mY == 0) || (directionsMovable == 2 && mX == 0) || (directionsMovable == 4 && ((mX < 0 && mY < 0)||(mX > 0 && mY > 0))) || (directionsMovable == 3 && ((mX < 0 && mY > 0)||(mX > 0 && mY < 0)))))
                            validOptions.push_back(Move(x, y, x+mX, y+mY, thisBoard->board[x+mX][y+mY].getPiece()));
                        break;
                    }
                    else if (directionsMovable == 0 || (directionsMovable == 1 && mY == 0) || (directionsMovable == 2 && mX == 0) || (directionsMovable == 4 && ((mX < 0 && mY < 0)||(mX > 0 && mY > 0))) || (directionsMovable == 3 && ((mX < 0 && mY > 0)||(mX > 0 && mY < 0))))
                        validOptions.push_back(Move(x, y, x+mX, y+mY, '.'));
                }
            }
        break;
        case 'n':
            if (directionsMovable == 0)
                for (auto& i : range)
                    if (inBound(x+knightXY[i].first, y+knightXY[i].second) && (getColour() != thisBoard->board[x+knightXY[i].first][y+knightXY[i].second].getColour() || thisBoard->board[x+knightXY[i].first][y+knightXY[i].second].getColour() == '.'))
                        validOptions.push_back(Move(x, y, x+knightXY[i].first, y+knightXY[i].second, thisBoard->board[x+knightXY[i].first][y+knightXY[i].second].getPiece()));
        break;
        case 'p':
            if (directionsMovable != 2)
            {
                int modifier = (getPiece() == 'p' ? 1 : -1);
                //check if en passant is possible by checking previous move (if the last move was a pawn) and current position of pawn selected
                if (!thisBoard->recordedMoves.empty() && thisBoard->board[thisBoard->recordedMoves.back().endX][thisBoard->recordedMoves.back().endY].getPiece() == oppositePiece[getPiece()] && thisBoard->recordedMoves.back().endX == (getPiece() == 'p' ? 4 : 3) && x == (getPiece() == 'p' ? 4 : 3))
                {
                    if (y+1 == thisBoard->recordedMoves.back().endY && directionsMovable != 3)
                        validOptions.push_back(Move(x, y, x+modifier, y+1, thisBoard->board[x][y+1].getPiece(), 1));
                    else if (y-1 == thisBoard->recordedMoves.back().endY && directionsMovable != 4)
                        validOptions.push_back(Move(x, y, x+modifier, y-1, thisBoard->board[x][y-1].getPiece(), 1));
                } 
                //normal move checking and also checks if initial two-step move is available
                if (inBound(x+modifier, y) && thisBoard->board[x+modifier][y].getPiece() == '.' && directionsMovable != 3 && directionsMovable != 4)
                {
                    validOptions.push_back(Move(x, y, x+modifier, y, '.', (modifier == 1 && x == 6) || (modifier == -1 && x == 1)));
                    if (thisBoard->board[x+(modifier*2)][y].getPiece() == '.' && ((modifier == 1 && x == 1) || (modifier == -1 && x == 6)))
                        validOptions.push_back(Move(x, y, x + (modifier*2), y, '.'));
                }
                //checks if the pawn can take any pieces
                if (inBound(x+modifier, y+1) && directionsMovable != 3 && thisBoard->board[x+modifier][y+1].getPiece() != '.' && getColour() != thisBoard->board[x+modifier][y+1].getColour())
                    validOptions.push_back(Move(x, y, x+modifier, y+1, thisBoard->board[x+modifier][y+1].getPiece(), ((modifier == 1 && x == 6) || (modifier == -1 && x == 1)) ? 2 : 0));
                if (inBound(x+modifier, y-1) && directionsMovable != 4 && thisBoard->board[x+modifier][y-1].getPiece() != '.' && getColour() != thisBoard->board[x+modifier][y-1].getColour())
                    validOptions.push_back(Move(x, y, x+modifier, y-1, thisBoard->board[x+modifier][y-1].getPiece(), ((modifier == 1 && x == 6) || (modifier == -1 && x == 1)) ? 2 : 0)); 
            }         
        break;
    }
}

//Private member functions
int Piece::isMovable() 
{
    vector<pair<int,int>> kingPos = thisBoard->returnPos(sameColour(getPiece(), 'k'));
    if (kingPos.empty()) return 0;
    int distX = pos.first - kingPos[0].first, distY = pos.second - kingPos[0].second, returnVal = 0;
    if (abs(distX) == abs(distY) || distX == 0 || distY == 0)
    {

        distX /= distX ? abs(distX) : 1;
        distY /= distY ? abs(distY) : 1;
        //checks if the king would be in check if the selected piece is moved by calling the inCheck method.
        //if the king will be in check, it returns a non-zero number to indicate the direction the piece can move to
        //still protect the king
        char tmpPiece = thisBoard->board[pos.first][pos.second].getPiece();
        setPiece('.');
        if (thisBoard->board[kingPos[0].first][kingPos[0].second].inCheck(tmpPiece).size() != 0)
        {
            if (distY == 0)
                returnVal = 1;
            else if (distX == 0)
                returnVal = 2;
            else if (distX * distY == -1)
                returnVal = 3;
            else if (distX * distY == 1)
                returnVal = 4;
        }
        setPiece(tmpPiece);
    }
    return returnVal; //0 = any direction, 1 = vertical, 2 = horizontal, 3 = diagonal up right, 4 = diagonal up left

}

vector<pair<int,int>> Piece::inCheck(const char side)
{
    vector<pair<int,int>> checks;
    for (auto& i : range)
    {
        //NOTE: not needed - only to shorten code
        Piece &current = thisBoard->board[pos.first+QBRKXY[i].first][pos.second+QBRKXY[i].second];
        //checks if the current position is attacked by a knight at (pos.first+knightXY[i].first, pos.second+knightXY[i].second)
        if (inBound(pos.first+knightXY[i].first, pos.second+knightXY[i].second) && thisBoard->board[pos.first+knightXY[i].first][pos.second+knightXY[i].second].getPiece() == oppositePiece[sameColour(side, 'n')])
            checks.push_back(make_pair(pos.first+knightXY[i].first, pos.second+knightXY[i].second));
        //checks if the current position is attacked by a pawn or a king
        //if it is, then it would mean no other pieces would be able to attack since the king/pawn is blocking the line of sight
        if (inBound(pos.first+QBRKXY[i].first, pos.second+QBRKXY[i].second)
                && ((((getColour(side) && QBRKXY[i].first == 1 && QBRKXY[i].second != 0) || (!getColour(side) && QBRKXY[i].first == -1 && QBRKXY[i].second != 0)) 
                && current.getPiece() == oppositePiece[sameColour(side, 'p')]) 
                || (current.getPiece() == oppositePiece[sameColour(side, 'k')] && ((current.inCheck(oppositePiece[side])).size() == 0))))
            checks.push_back(make_pair(pos.first+QBRKXY[i].first, pos.second+QBRKXY[i].second));
        //if the current position is not attacked by a king of pawn, check if a bishop/rook/queen is attacking the current position
        else
        {
            for (unsigned int j = 1; j < 8; j++)
            {
                int mX = (j*QBRKXY[i].first) + pos.first, mY = (j*QBRKXY[i].second) + pos.second;
                if (!inBound(mX, mY))
                    break;
                //if the current posiiton is attacked by a rook and the global iteration count is less than 4,
                //or if it is attacked by a bishop and the iteraton count is greater than 3, or if it is attacked by a queen
                //add the piece to the vector
                else if ((thisBoard->board[mX][mY].getPiece() == oppositePiece[sameColour(side, 'r')] && i < 4) || (thisBoard->board[mX][mY].getPiece() == oppositePiece[sameColour(side, 'b')] && i > 3) || thisBoard->board[mX][mY].getPiece() == oppositePiece[sameColour(side, 'q')])
                {
                    checks.push_back(make_pair(mX, mY));
                    break;
                }
                else if (thisBoard->board[mX][mY].getPiece() != '.')
                    break;
            }
        }
    }
    return checks;
}
