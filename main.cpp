/*TODO LIST
 *Add 3-time repetition
 *Allow game state saving and reloading
 *??Add option to end game anytime
 *??Add option to choose another piece to move
 *Check for bugs
 */
#include <algorithm>
#include <bitset>
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

unordered_map<char,char> oppositePiece = {{'r', 'R'}, {'R', 'r'},
                                          {'n', 'N'}, {'N', 'n'},
                                          {'b', 'B'}, {'B', 'b'},
                                          {'q', 'Q'}, {'Q', 'q'},
                                          {'k', 'K'}, {'K', 'k'},
                                          {'p', 'P'}, {'P', 'p'},
                                          {'.','.'}};

pair<int,int> knightXY[8] = {make_pair(2, 1), make_pair(2, -1), make_pair(-2, 1), make_pair(-2, -1), 
                             make_pair(1, 2), make_pair(1, -2), make_pair(-1, 2), make_pair(-1, -2)};

pair<int,int> QBRKXY[8] = {make_pair(1, 0), make_pair(-1, 0), make_pair(0, 1), make_pair(0, -1), 
                           make_pair(1, 1), make_pair(1, -1), make_pair(-1, 1), make_pair(-1, -1)};

template <typename T>
void getNum (T &x)
{
    while(!(cin >> x))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout << "Please enter a number: ";
    }
}

struct Move;

class Piece
{
public:
    Piece() = default;
    static Piece make_piece(int, int, int, char);
    static Piece make_piece(int, int, char);    
    void getMoves (vector<Move> &);
    static void getPieceMoves(char, vector<Move> &);
    int getColour ();
    void setColour (int);
    pair<int,int> getPos();
    void setPos (int, int);
    char getPiece ();
    void setPiece (char); 
    static int getColour (char piece) {return (piece == '.' ? 2 : (piece == 'q' || piece == 'r' || piece == 'b' || piece == 'n' || piece == 'k' || piece == 'p' ? 1 : 0));}

    int isMovable();
    vector<pair<int,int>> inCheck(char);
    void checkedMoves(char, vector<Move> &);
private:
    Piece(int, int, int, char);
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
private:
};

Board game;

inline bool inBound (int posX, int posY) {return (posX < 8 && posX >= 0 && posY < 8 && posY >= 0);}

inline char sameColour(char basePiece, char piece) {return Piece::getColour(basePiece) == Piece::getColour(piece) ? piece : oppositePiece[piece];}

struct Move
{
    bool castle = false;
    int startX, startY, endX, endY, rookStartX, rookStartY, rookEndX, rookEndY, pawnSpecial = 0; //0 = nothing, 1 = en passant, 2 = promotion
    char takenPiece = '.';
    Move(int sX, int sY, int eX, int eY, char pp)
    {
        startX = sX, startY = sY, endX = eX, endY = eY;
        takenPiece = pp;
    }
    Move(int sX, int sY, int eX, int eY, char pp, int s)
    {
        startX = sX, startY = sY, endX = eX, endY = eY;
        takenPiece = pp, pawnSpecial = s;
    }
    Move(int sX, int sY, int eX, int eY, int rsX, int rsY, int reX, int reY, bool c)
    {
        startX = sX, startY = sY, endX = eX, endY = eY;
        rookStartX = rsX, rookStartY = rsY, rookEndX = reX, rookEndY = reY;
        castle = c;
    }
    void applyMove()
    {
        game.board[endX][endY].setPiece(game.board[startX][startY].getPiece());
        game.board[startX][startY].setPiece('.');
        game.moved[startX][startY] = true;
        if (castle)
        {
            game.board[rookEndX][rookEndY].setPiece(game.board[rookStartX][rookStartY].getPiece());
            game.board[rookStartX][rookStartY].setPiece('.');
            game.moved[rookStartX][rookStartY] = true;
        }
        else if (pawnSpecial == 1)
            game.board[endX + (game.board[endX][endY].getPiece() == 'p' ? -1 : 1)][endY].setPiece('.');
        else if (pawnSpecial == 2)
        {
            char promotedPiece, boardPiece = game.board[endX][endY].getPiece();
            cout <<"Promote piece to: "<<sameColour(boardPiece, 'n')<<" "<<sameColour(boardPiece, 'b')<<" "<<sameColour(boardPiece, 'r')<<" "<<sameColour(boardPiece, 'q')<<endl;
            cin>>promotedPiece;
            while (promotedPiece != sameColour(boardPiece, 'n') && promotedPiece != sameColour(boardPiece, 'b') && promotedPiece != sameColour(boardPiece, 'r') && promotedPiece != sameColour(boardPiece, 'q'))
            {
                cout<<"Invalid piece for promotion (Tip: The input is case-sensitive)"<<endl;
                cin>>promotedPiece;
            }
            game.board[endX][endY].setPiece(promotedPiece);
        }       
    }
    void undoMove()
    {
        game.board[startX][startY].setPiece(game.board[endX][endY].getPiece());
        game.board[endX][endY].setPiece('.');
        game.moved[startX][startY] = false;
        if (castle)
        {
            game.board[rookStartX][rookStartY].setPiece(game.board[rookEndX][rookEndY].getPiece());
            game.board[rookEndX][rookEndY].setPiece('.');
            game.moved[rookStartX][rookStartY] = false;
        }
        else if (pawnSpecial == 1)
            game.board[startX][endY].setPiece(oppositePiece[game.board[startX][startY].getPiece()]);
        else
        {
            game.board[endX][endY].setPiece(takenPiece);
            game.board[endX][endY].setColour(!game.board[endX][endY].getColour());  
            if (pawnSpecial == 2)
                game.board[startX][startY].setPiece(sameColour(game.board[startX][startY].getPiece(), 'p'));
        }
    }
};

vector<Move> recordedMoves;

vector<pair<int,int>> returnPos (char piece)
{
    vector<pair<int,int>> pos;
    for (unsigned int x = 0; x < 8; x++)
        for (unsigned int y = 0; y < 8; y++)
            if (game.board[x][y].getPiece() == piece)
                pos.push_back(make_pair(x,y));
    return pos;
}

Piece::Piece(int colour, int x, int y, char type) 
{   
    this->colour = colour, pos = make_pair(x, y), this->type = tolower(type);
}

Piece Piece::make_piece(int colour, int x, int y, char type) 
{
    return Piece(colour, x, y, type);
}

Piece Piece::make_piece(int x, int y, char type) 
{
    return Piece(2, x, y, type);
}

int Piece::getColour () { return colour; }
void Piece::setColour (int colour) { this->colour = colour; }
pair<int,int> Piece::getPos() { return pos; }
void Piece::setPos (int x, int y) { pos = make_pair(x, y); }
char Piece::getPiece () { return (getColour(type) == colour ? type : oppositePiece[type]); }
void Piece::setPiece (char type) 
{
    this->type = tolower(type);
    if (this->type == '.') setColour(2);
    else setColour(getColour(type));
}

//0 = any direction, 1 = vertical, 2 = horizontal, 3 = diagonal up right, 4 = diagonal up left
int Piece::isMovable()
{
    vector<pair<int,int>> kingPos = returnPos(sameColour(this->getPiece(), 'k'));
    if (kingPos.empty()) return 0;
    int distX = pos.first - kingPos[0].first, distY = pos.second - kingPos[0].second, returnVal = 0;
    if (abs(distX) == abs(distY) || distX == 0 || distY == 0)
    {

        distX /= distX ? abs(distX) : 1;
        distY /= distY ? abs(distY) : 1;
        char tmpPiece = game.board[pos.first][pos.second].getPiece();
        this->setPiece('.');
        if (game.board[kingPos[0].first][kingPos[0].second].inCheck(tmpPiece).size() != 0)
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
        Piece &current = game.board[pos.first+QBRKXY[i].first][pos.second+QBRKXY[i].second];
        if (inBound(pos.first+knightXY[i].first, pos.second+knightXY[i].second) && game.board[pos.first+knightXY[i].first][pos.second+knightXY[i].second].getPiece() == oppositePiece[sameColour(side, 'n')])
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
                else if ((game.board[mX][mY].getPiece() == oppositePiece[sameColour(side, 'r')] && i < 4) || (game.board[mX][mY].getPiece() == oppositePiece[sameColour(side, 'b')] && i > 3) || game.board[mX][mY].getPiece() == oppositePiece[sameColour(side, 'q')])
                {
                    checks.push_back(make_pair(mX, mY));
                    break;
                }
                else if (game.board[mX][mY].getPiece() != '.')
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
            if (game.board[x][y+1].inCheck(type).size() == 0 && game.board[x][y+2].inCheck(type).size() == 0 && game.board[x][y+1].getPiece() == '.' && game.board[x][y+2].getPiece() == '.' && !game.moved[x][7] && !game.moved[x][y])
                validOptions.push_back(Move(x, y, x, y+2, x, 7, x, y+1, true));
            if (game.board[x][y-1].inCheck(type).size() == 0 && game.board[x][y-2].inCheck(type).size() == 0 && game.board[x][y-1].getPiece() == '.' && game.board[x][y-2].getPiece() == '.' && !game.moved[x][0] && !game.moved[x][y])
                validOptions.push_back(Move(x, y, x, y-2, x, 0, x, y-1, true));
            for (unsigned int i = 0; i < 8; i++)
                if (inBound(x+QBRKXY[i].first, y+QBRKXY[i].second) && (game.board[x+QBRKXY[i].first][y+QBRKXY[i].second].getColour() != this->getColour() || game.board[x+QBRKXY[i].first][y+QBRKXY[i].second].getPiece() == '.') && game.board[x+QBRKXY[i].first][y+QBRKXY[i].second].inCheck(type).size() == 0)
                    validOptions.push_back(Move(x, y, x+QBRKXY[i].first, y+QBRKXY[i].second, game.board[x+QBRKXY[i].first][y+QBRKXY[i].second].getPiece()));
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
                    if (game.board[x+mX][y+mY].getPiece() != '.')
                    {
                        if (this->getColour() != game.board[x+mX][y+mY].getColour() && (directionsMovable == 0 || (directionsMovable == 1 && mY == 0) || (directionsMovable == 2 && mX == 0) || (directionsMovable == 4 && ((mX < 0 && mY < 0)||(mX > 0 && mY > 0))) || (directionsMovable == 3 && ((mX < 0 && mY > 0)||(mX > 0 && mY < 0)))))
                            validOptions.push_back(Move(x, y, x+mX, y+mY, game.board[x+mX][y+mY].getPiece()));
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
                    if (inBound(x+knightXY[i].first, y+knightXY[i].second) && (this->getColour() != game.board[x+knightXY[i].first][y+knightXY[i].second].getColour() || game.board[x+knightXY[i].first][y+knightXY[i].second].getColour() == '.'))
                        validOptions.push_back(Move(x, y, x+knightXY[i].first, y+knightXY[i].second, game.board[x+knightXY[i].first][y+knightXY[i].second].getPiece()));
        break;
        case 'p':
            if (directionsMovable != 2)
            {
                int modifier = (this->getPiece() == 'p' ? 1 : -1);
                if (!recordedMoves.empty() && game.board[recordedMoves.back().endX][recordedMoves.back().endY].getPiece() == oppositePiece[this->getPiece()] && recordedMoves.back().endX == (this->getPiece() == 'p' ? 4 : 3) && x == (this->getPiece() == 'p' ? 4 : 3))
                {
                    if (y+1 == recordedMoves.back().endY && directionsMovable != 3)
                        validOptions.push_back(Move(x, y, x+modifier, y+1, game.board[x][y+1].getPiece(), 1));
                    else if (y-1 == recordedMoves.back().endY && directionsMovable != 4)
                        validOptions.push_back(Move(x, y, x+modifier, y-1, game.board[x][y-1].getPiece(), 1));
                } 
                if (inBound(x+modifier, y) && game.board[x+modifier][y].getPiece() == '.' && directionsMovable != 3 && directionsMovable != 4)
                {
                    validOptions.push_back(Move(x, y, x+modifier, y, '.', (modifier == 1 && x == 6) || (modifier == -1 && x == 1)));
                    if (game.board[x+(modifier*2)][y].getPiece() == '.' && ((modifier == 1 && x == 1) || (modifier == -1 && x == 6)))
                        validOptions.push_back(Move(x, y, x + (modifier*2), y, '.'));
                }
                if (inBound(x+modifier, y+1) && directionsMovable != 3 && game.board[x+modifier][y+1].getPiece() != '.' && this->getColour() != game.board[x+modifier][y+1].getColour())
                    validOptions.push_back(Move(x, y, x+modifier, y+1, game.board[x+modifier][y+1].getPiece(), ((modifier == 1 && x == 6) || (modifier == -1 && x == 1)) ? 2 : 0));
                if (inBound(x+modifier, y-1) && directionsMovable != 4 && game.board[x+modifier][y-1].getPiece() != '.' && this->getColour() != game.board[x+modifier][y-1].getColour())
                    validOptions.push_back(Move(x, y, x+modifier, y-1, game.board[x+modifier][y-1].getPiece(), ((modifier == 1 && x == 6) || (modifier == -1 && x == 1)) ? 2 : 0)); 
            }         
        break;
    }
}

void Piece::checkedMoves(char side, vector<Move> &validOptions)
{
    vector<pair<int,int>> tmpPos = returnPos(sameColour(side, 'k'));
    int distX = tmpPos[0].first - pos.first, distY = tmpPos[0].second - pos.second, x = pos.first, y = pos.second, xx = tmpPos[0].first, yy = tmpPos[0].second;
    distX /= distX ? abs(distX) : 1;
    distY /= distY ? abs(distY) : 1;
    for (unsigned int i = 0; i < 8 && inBound(x, y) && (x != xx || y != yy) && (tolower(game.board[pos.first][pos.second].getPiece()) != 'n' || i == 0); i++)
    {
        tmpPos = game.board[x][y].inCheck(game.board[pos.first][pos.second].getPiece());
        for (pair<int,int>& j : tmpPos)
            if (tolower(game.board[j.first][j.second].getPiece()) != 'p' || (pos.first == x && pos.second == y))
                validOptions.push_back(Move(j.first, j.second, x, y, game.board[x][y].getPiece()));
        if ((getColour(side) && game.board[x-1][y].getPiece() == 'p') || (!getColour(side) && game.board[x+1][y].getPiece() == 'P'))
            validOptions.push_back(Move(x + (getColour(side) ? -1 : 1), y, x, y, game.board[x][y].getPiece()));
        x += distX, y += distY;
    }
}

void Piece::getPieceMoves(char piece, vector<Move> &validOptions)
{
    vector<pair<int,int>> pos = returnPos(piece);
    for (pair<int,int>& z : pos)
        game.board[z.first][z.second].getMoves(validOptions);
}

Board::Board()
{
    char start [8][8] = {{'r','n','b','q','k','b','n','r'},
                         {'p','p','p','p','p','p','p','p'},
                         {'.','.','.','.','.','.','.','.'},
                         {'.','.','.','.','.','.','.','.'},
                         {'.','.','.','.','.','.','.','.'},
                         {'.','.','.','.','.','.','.','.'},
                         {'P','P','P','P','P','P','P','P'},
                         {'R','N','B','Q','K','B','N','R'}};
    for (unsigned int x = 0; x < 8; x++)
        for (unsigned int y = 0; y < 8; y++)
            board[x][y] = Piece::make_piece(Piece::getColour(start[x][y]), x, y, tolower(start[x][y]));
}

void Board::displayBoard(const bool side)
{
    int startX = (side ? 11 : 0), endX = (side ? -1 : 12);
    cout<<endl;
    for (unsigned int x = startX; x != endX; x += (endX-startX)/abs(endX-startX))
    {
        if (x == 1 || x == 10)
            cout<<"~+~~~~~~~~+~"<<endl;
        else if (x == 0 || x == 11)
            cout<<"X|01234567|X"<<endl;
        else
        {
            cout <<x-2<<"|";
            for (unsigned int y = 0; y < 8; y++)
                cout<<this->board[x-2][y].getPiece();
            cout<<"|"<<x-2<<endl;
        }
    }    
}


bool stalemate(bool colour)
{
    vector<Move> tmp;
    for (unsigned int x = 0; x  < 8 && tmp.empty(); x++)
        for (unsigned int y = 0; y < 8 && tmp.empty(); y++)
            if (game.board[x][y].getColour() == colour && game.board[x][y].getPiece() != '.')
                game.board[x][y].getMoves(tmp);
    return tmp.empty();
}

bool repetition()
{

}

int check(char piece, vector<Move> &validOptions)
{
    vector<pair<int,int>> checker = returnPos(sameColour(piece, 'k')); 
    if (checker.empty()) return 1;
    bool noMovesLeft = true;
    for (unsigned int i = 0; i < 8 && noMovesLeft; i++)
        if (inBound(checker[0].first+QBRKXY[i].first, checker[0].second+QBRKXY[i].second) && (game.board[checker[0].first+QBRKXY[i].first][checker[0].second+QBRKXY[i].second].getColour() != game.board[checker[0].first][checker[0].second].getColour() || game.board[checker[0].first+QBRKXY[i].first][checker[0].second+QBRKXY[i].second].getPiece() == '.') && game.board[checker[0].first+QBRKXY[i].first][checker[0].second+QBRKXY[i].second].inCheck(game.board[checker[0].first][checker[0].second].getPiece()).size() == 0)
            noMovesLeft = false;
    checker = game.board[checker[0].first][checker[0].second].inCheck(game.board[checker[0].first][checker[0].second].getPiece());
    if (checker.size() > 1)
    {
        if (noMovesLeft)
            return 0;
        return -1;
    }
    else if (checker.size() == 1)
    {
        vector<Move> tmpValidOptions;
        game.board[checker[0].first][checker[0].second].checkedMoves(piece, tmpValidOptions);
        if (tmpValidOptions.empty() && noMovesLeft)
            return 0;
        else
        {
            for (Move& i : tmpValidOptions)
                if (game.board[i.startX][i.startY].getPiece() == piece)
                    validOptions.push_back(i);
            return -1;
        }
    }
    return 1;
}

bool displayMoves(vector<Move> &validOptions)
{
    if (!validOptions.empty())
    {
        for (Move& y : validOptions)
            cout<<y.startX<<" "<<y.startY<<" "<<y.endX<<" "<<y.endY<<" "<<(y.castle ? "(castle) " : (y.takenPiece == '.' ? " " : y.pawnSpecial == 1 ? "(en passant) " : "(take piece) "))
                <<(y.pawnSpecial == 2 ? "(promote pawn " : " ")<<endl;
        int sX, sY, eX, eY, pointer;
        bool valid = false;
        while (!valid)
        {
            cout<<"Enter start X position (0 - 7): ";
            getNum<int>(sX);
            cout<<"Enter start Y position (0 - 7): ";
            getNum<int>(sY);
            cout<<"Enter end X position (0 - 7): ";
            getNum<int>(eX);
            cout<<"Enter end Y position (0 - 7): ";
            getNum<int>(eY);
            for (unsigned int x = 0; x < validOptions.size() && !valid; x++)
                if (validOptions[x].startX == sX && validOptions[x].startY == sY && validOptions[x].endX == eX && validOptions[x].endY == eY)
                    valid = true, pointer = x;
                else if (x == validOptions.size() - 1)
                    cout<<"Invalid position. Please try again."<<endl;
        }
        recordedMoves.push_back(validOptions[pointer]);
        validOptions[pointer].applyMove();
    }
    else
    {
        cout<<"No valid moves available for this piece."<<endl;
        return false;
    }
    return true;
}

int main()
{
    char piece;
    bool playerTurn = true, loop = true;;
    int checkmate, exitReason = 255; // 0 = checkmate, 1 = stalemate, 2 = draw, 3 = resign, 4 = exit, 255 = unknown
    vector<Move> options;
    while (exitReason == 255)
    {
        options.clear();
        playerTurn ^= loop;
        game.displayBoard(playerTurn);
        checkmate = check(playerTurn ? 'k' : 'K', options);
        if (checkmate == 0 || stalemate(playerTurn))
        {
            exitReason = (checkmate == 0 ? 0 : 1);
            break;
        }
        cout<<(playerTurn ? "Black" : "White")<<"'s turn. Enter a piece to move. Enter \"h\" for help."<<endl;
        cin>>piece;
        piece = (playerTurn) ? tolower(piece) : toupper(piece);
        loop = false;
        switch(tolower(piece))
        {
            case 'r':
            case 'b':
            case 'n':
            case 'q':
            case 'k':
            case 'p':
                if (checkmate == 1 || tolower(piece) == 'k')
                    Piece::getPieceMoves(piece, options);
                loop = displayMoves(options);
            break;
            case 'u':
                if (recordedMoves.empty())
                    cout<<"No moves have been done yet."<<endl;
                else
                {                
                    cout<<"Are you sure you want to undo?"<<endl;
                    cin>>piece;
                    if (tolower(piece) == 'y')
                    {
                        recordedMoves.back().undoMove();
                        recordedMoves.pop_back();
                        loop = true;
                        cout<<"Move undone."<<endl;
                    }
                }
            break;
            case 'd':
                cout<<(playerTurn ? "Black": "White")<<" wants to draw. "<<(playerTurn ? "White" : "Black")<<" - do you want to draw?"<<endl;
                cin>>piece;
                if (tolower(piece) == 'y') 
                    exitReason = 2;
            break;
            case 'g':
                cout<<"Are you sure you want to give up?"<<endl;
                cin>>piece;
                if (tolower(piece) == 'y') 
                    exitReason = 3;
            break;
            case 'h':
                cout<<"+~~~~~~~~~~~~~~~~~~~~+"<<endl;
                cout<<"|\"u\" - undo a move.  |"<<endl;
                cout<<"|\"d\" - offer draw.   |"<<endl;
                cout<<"|\"g\" - give up.      |"<<endl;
                cout<<"|\"r\" - move a Rook.  |"<<endl;
                cout<<"|\"b\" - move a Bishop.|"<<endl;
                cout<<"|\"n\" - move a Knight.|"<<endl;
                cout<<"|\"q\" - move a Queen. |"<<endl;
                cout<<"|\"k\" - move the King.|"<<endl;
                cout<<"|\"p\" - move a Pawn.  |"<<endl;
                cout<<"+~~~~~~~~~~~~~~~~~~~~+"<<endl;
            break;
            default:
                cout<<"Invalid piece selected."<<endl;
            break;
        }
    }
    switch(exitReason)
    {
        case 0:
            cout<<(playerTurn ? "Black" : "White")<<" is checkmated!"<<endl;
        break;
        case 1:
            cout<<"Stalemate! No one wins!"<<endl;
        break;
        case 2:
            cout<<"A draw has been agreed! No one wins!"<<endl;
        break;
        case 3:
            cout<<(playerTurn ? "Black" : "White")<<" resigns!"<<endl;
        break;
    }
    return 0;
}