#include "Chess.h"

Piece::Piece(int colour, int x, int y, char type, Board* location) 
{   
    this->colour = colour, pos = make_pair(x, y), this->type = tolower(type), this->thisBoard = location;
    assert(this->thisBoard != nullptr);
}

Piece Piece::make_piece(int colour, int x, int y, char type, Board* location) { return Piece(colour, x, y, type, location); }
Piece Piece::make_piece(int x, int y, char type, Board* location) { return Piece(2, x, y, type, location); }
Piece Piece::make_piece(Board* location) { return Piece(2, 8, 8, '.', location); }
int Piece::getColour () { return colour; }
void Piece::setColour (int colour) { this->colour = colour; }
pair<int,int> Piece::getPos() { return pos; }
void Piece::setPos (int x, int y) { pos = make_pair(x, y); }
char Piece::getPiece () { return (getColour(type) == colour ? type : oppositePiece[type]); }
void Piece::setPiece (char type) 
{
    this->type = tolower(type);
    if (this->type == '.') this->colour = 2;
    else this->colour = Piece::getColour(type);
}

//0 = any direction, 1 = vertical, 2 = horizontal, 3 = diagonal up right, 4 = diagonal up left
int Piece::isMovable()
{
    vector<pair<int,int>> kingPos = thisBoard->returnPos(sameColour(this->getPiece(), 'k'));
    if (kingPos.empty()) return 0;
    int distX = pos.first - kingPos[0].first, distY = pos.second - kingPos[0].second, returnVal = 0;
    if (abs(distX) == abs(distY) || distX == 0 || distY == 0)
    {

        distX /= distX ? abs(distX) : 1;
        distY /= distY ? abs(distY) : 1;
        char tmpPiece = thisBoard->board[pos.first][pos.second].getPiece();
        this->setPiece('.');
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
        this->setPiece(tmpPiece);
    }
    return returnVal;
}

vector<pair<int,int>> Piece::inCheck(char side)
{
    vector<pair<int,int>> checks;
    for (unsigned int i = 0; i < 8; i++)
    {
        Piece &current = thisBoard->board[pos.first+QBRKXY[i].first][pos.second+QBRKXY[i].second];
        if (inBound(pos.first+knightXY[i].first, pos.second+knightXY[i].second) && thisBoard->board[pos.first+knightXY[i].first][pos.second+knightXY[i].second].getPiece() == oppositePiece[sameColour(side, 'n')])
            checks.push_back(make_pair(pos.first+knightXY[i].first, pos.second+knightXY[i].second));
        if (inBound(pos.first+QBRKXY[i].first, pos.second+QBRKXY[i].second)
                && ((((getColour(side) && QBRKXY[i].first == 1 && QBRKXY[i].second != 0) || (!getColour(side) && QBRKXY[i].first == -1 && QBRKXY[i].second != 0)) 
                && current.getPiece() == oppositePiece[sameColour(side, 'p')]) 
                || (current.getPiece() == oppositePiece[sameColour(side, 'k')] && ((current.inCheck(oppositePiece[side])).size() == 0))))
            checks.push_back(make_pair(pos.first+QBRKXY[i].first, pos.second+QBRKXY[i].second));
        else
        {
            for (unsigned int j = 1; j < 8; j++)
            {
                int mX = (j*QBRKXY[i].first) + pos.first, mY = (j*QBRKXY[i].second) + pos.second;
                if (!inBound(mX, mY))
                    break;
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

void Piece::getMoves(vector<Move> &validOptions)
{
    int directionsMovable = this->isMovable(), x = pos.first, y = pos.second;
    switch(tolower(type))
    {
        case 'k':
            if (thisBoard->board[x][y+1].inCheck(type).size() == 0 && thisBoard->board[x][y+2].inCheck(type).size() == 0 && thisBoard->board[x][y+1].getPiece() == '.' && thisBoard->board[x][y+2].getPiece() == '.' && !thisBoard->moved[x][7] && !thisBoard->moved[x][y])
                validOptions.push_back(Move(x, y, x, y+2, x, 7, x, y+1, true));
            if (thisBoard->board[x][y-1].inCheck(type).size() == 0 && thisBoard->board[x][y-2].inCheck(type).size() == 0 && thisBoard->board[x][y-1].getPiece() == '.' && thisBoard->board[x][y-2].getPiece() == '.' && !thisBoard->moved[x][0] && !thisBoard->moved[x][y])
                validOptions.push_back(Move(x, y, x, y-2, x, 0, x, y-1, true));
            for (unsigned int i = 0; i < 8; i++)
                if (inBound(x+QBRKXY[i].first, y+QBRKXY[i].second) && (thisBoard->board[x+QBRKXY[i].first][y+QBRKXY[i].second].getColour() != this->getColour() || thisBoard->board[x+QBRKXY[i].first][y+QBRKXY[i].second].getPiece() == '.') && thisBoard->board[x+QBRKXY[i].first][y+QBRKXY[i].second].inCheck(type).size() == 0)
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
                        if (this->getColour() != thisBoard->board[x+mX][y+mY].getColour() && (directionsMovable == 0 || (directionsMovable == 1 && mY == 0) || (directionsMovable == 2 && mX == 0) || (directionsMovable == 4 && ((mX < 0 && mY < 0)||(mX > 0 && mY > 0))) || (directionsMovable == 3 && ((mX < 0 && mY > 0)||(mX > 0 && mY < 0)))))
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
                for (unsigned int i = 0; i < 8; i++)
                    if (inBound(x+knightXY[i].first, y+knightXY[i].second) && (this->getColour() != thisBoard->board[x+knightXY[i].first][y+knightXY[i].second].getColour() || thisBoard->board[x+knightXY[i].first][y+knightXY[i].second].getColour() == '.'))
                        validOptions.push_back(Move(x, y, x+knightXY[i].first, y+knightXY[i].second, thisBoard->board[x+knightXY[i].first][y+knightXY[i].second].getPiece()));
        break;
        case 'p':
            if (directionsMovable != 2)
            {
                int modifier = (this->getPiece() == 'p' ? 1 : -1);
                if (!thisBoard->recordedMoves.empty() && thisBoard->board[thisBoard->recordedMoves.back().endX][thisBoard->recordedMoves.back().endY].getPiece() == oppositePiece[this->getPiece()] && thisBoard->recordedMoves.back().endX == (this->getPiece() == 'p' ? 4 : 3) && x == (this->getPiece() == 'p' ? 4 : 3))
                {
                    if (y+1 == thisBoard->recordedMoves.back().endY && directionsMovable != 3)
                        validOptions.push_back(Move(x, y, x+modifier, y+1, thisBoard->board[x][y+1].getPiece(), 1));
                    else if (y-1 == thisBoard->recordedMoves.back().endY && directionsMovable != 4)
                        validOptions.push_back(Move(x, y, x+modifier, y-1, thisBoard->board[x][y-1].getPiece(), 1));
                } 
                if (inBound(x+modifier, y) && thisBoard->board[x+modifier][y].getPiece() == '.' && directionsMovable != 3 && directionsMovable != 4)
                {
                    validOptions.push_back(Move(x, y, x+modifier, y, '.', (modifier == 1 && x == 6) || (modifier == -1 && x == 1)));
                    if (thisBoard->board[x+(modifier*2)][y].getPiece() == '.' && ((modifier == 1 && x == 1) || (modifier == -1 && x == 6)))
                        validOptions.push_back(Move(x, y, x + (modifier*2), y, '.'));
                }
                if (inBound(x+modifier, y+1) && directionsMovable != 3 && thisBoard->board[x+modifier][y+1].getPiece() != '.' && this->getColour() != thisBoard->board[x+modifier][y+1].getColour())
                    validOptions.push_back(Move(x, y, x+modifier, y+1, thisBoard->board[x+modifier][y+1].getPiece(), ((modifier == 1 && x == 6) || (modifier == -1 && x == 1)) ? 2 : 0));
                if (inBound(x+modifier, y-1) && directionsMovable != 4 && thisBoard->board[x+modifier][y-1].getPiece() != '.' && this->getColour() != thisBoard->board[x+modifier][y-1].getColour())
                    validOptions.push_back(Move(x, y, x+modifier, y-1, thisBoard->board[x+modifier][y-1].getPiece(), ((modifier == 1 && x == 6) || (modifier == -1 && x == 1)) ? 2 : 0)); 
            }         
        break;
    }
}

void Piece::checkedMoves(char side, vector<Move> &validOptions)
{
    vector<pair<int,int>> tmpPos = thisBoard->returnPos(sameColour(side, 'k'));
    int distX = tmpPos[0].first - pos.first, distY = tmpPos[0].second - pos.second, x = pos.first, y = pos.second, xx = tmpPos[0].first, yy = tmpPos[0].second;
    distX /= distX ? abs(distX) : 1;
    distY /= distY ? abs(distY) : 1;
    for (unsigned int i = 0; i < 8 && inBound(x, y) && (x != xx || y != yy) && (tolower(thisBoard->board[pos.first][pos.second].getPiece()) != 'n' || i == 0); i++)
    {
        tmpPos = thisBoard->board[x][y].inCheck(thisBoard->board[pos.first][pos.second].getPiece());
        for (pair<int,int>& j : tmpPos)
            if (tolower(thisBoard->board[j.first][j.second].getPiece()) != 'p' || (pos.first == x && pos.second == y))
                validOptions.push_back(Move(j.first, j.second, x, y, thisBoard->board[x][y].getPiece()));
        if ((getColour(side) && thisBoard->board[x-1][y].getPiece() == 'p') || (!getColour(side) && thisBoard->board[x+1][y].getPiece() == 'P'))
            validOptions.push_back(Move(x + (getColour(side) ? -1 : 1), y, x, y, thisBoard->board[x][y].getPiece()));
        x += distX, y += distY;
    }
}

