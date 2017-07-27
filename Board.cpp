#include "Chess.h"

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
            board[x][y] = Piece::make_piece(Piece::getColour(start[x][y]), x, y, tolower(start[x][y]), this);
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

bool Board::stalemate (bool colour)
{
    vector<Move> tmp;
    for (unsigned int x = 0; x  < 8 && tmp.empty(); x++)
        for (unsigned int y = 0; y < 8 && tmp.empty(); y++)
            if (this->board[x][y].getColour() == colour && this->board[x][y].getPiece() != '.')
                this->board[x][y].getMoves(tmp);
    return tmp.empty();
}

bool Board::repetition()
{

}

int Board::checkmate (char piece, vector<Move> &validOptions)
{
    vector<pair<int,int>> checker = returnPos(sameColour(piece, 'k')); 
    if (checker.empty()) return 1;
    bool noMovesLeft = true;
    for (unsigned int i = 0; i < 8 && noMovesLeft; i++)
        if (inBound(checker[0].first+QBRKXY[i].first, checker[0].second+QBRKXY[i].second) && (this->board[checker[0].first+QBRKXY[i].first][checker[0].second+QBRKXY[i].second].getColour() != this->board[checker[0].first][checker[0].second].getColour() || this->board[checker[0].first+QBRKXY[i].first][checker[0].second+QBRKXY[i].second].getPiece() == '.') && this->board[checker[0].first+QBRKXY[i].first][checker[0].second+QBRKXY[i].second].inCheck(this->board[checker[0].first][checker[0].second].getPiece()).size() == 0)
            noMovesLeft = false;
    checker = this->board[checker[0].first][checker[0].second].inCheck(this->board[checker[0].first][checker[0].second].getPiece());
    if (checker.size() > 1)
    {
        if (noMovesLeft)
            return 0;
        return -1;
    }
    else if (checker.size() == 1)
    {
        vector<Move> tmpValidOptions;
        this->board[checker[0].first][checker[0].second].checkedMoves(piece, tmpValidOptions);
        if (tmpValidOptions.empty() && noMovesLeft)
            return 0;
        else
        {
            for (Move& i : tmpValidOptions)
                if (this->board[i.startX][i.startY].getPiece() == piece)
                    validOptions.push_back(i);
            return -1;
        }
    }
    return 1;
}

void Board::getPieceMoves(char piece, vector<Move> &validOptions)
{
    vector<pair<int,int>> pos = returnPos(piece);
    for (pair<int,int>& z : pos)
        this->board[z.first][z.second].getMoves(validOptions);
}

bool Board::displayPieceMoves(vector<Move> &validOptions)
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
        applyMove(recordedMoves.back());
    }
    else
    {
        cout<<"No valid moves available for this piece."<<endl;
        return false;
    }
    return true;
}

vector<pair<int,int>> Board::returnPos (char piece)
{
    vector<pair<int,int>> pos;
    for (unsigned int x = 0; x < 8; x++)
        for (unsigned int y = 0; y < 8; y++)
            if (this->board[x][y].getPiece() == piece)
                pos.push_back(make_pair(x,y));
    return pos;
}

void Board::updateBoard()
{
    for (unsigned int x = 0; x < 8; x++)
        for (unsigned int y = 0; y < 8; y++)
            this->board[x][y].setPos(x, y);
}

void Board::applyMove(Move& chosen)
{
    this->board[chosen.endX][chosen.endY] = this->board[chosen.startX][chosen.startY];
    this->board[chosen.startX][chosen.startY] = Piece::make_piece(this);
    this->moved[chosen.startX][chosen.startY] = true;
    if (chosen.castle)
    {
        this->board[chosen.rookEndX][chosen.rookEndY] = this->board[chosen.rookStartX][chosen.rookStartY];
        this->board[chosen.rookStartX][chosen.rookStartY] = Piece::make_piece(this);
        this->moved[chosen.rookStartX][chosen.rookStartY] = true;
    }
    else if (chosen.pawnSpecial == 1)
        this->board[chosen.endX + (this->board[chosen.endX][chosen.endY].getPiece() == 'p' ? -1 : 1)][chosen.endY] = Piece::make_piece(this);
    else if (chosen.pawnSpecial == 2)
    {
        char promotedPiece, boardPiece = this->board[chosen.endX][chosen.endY].getPiece();
        cout <<"Promote piece to: "<<sameColour(boardPiece, 'n')<<" "<<sameColour(boardPiece, 'b')<<" "<<sameColour(boardPiece, 'r')<<" "<<sameColour(boardPiece, 'q')<<endl;
        cin>>promotedPiece;
        while (promotedPiece != sameColour(boardPiece, 'n') && promotedPiece != sameColour(boardPiece, 'b') && promotedPiece != sameColour(boardPiece, 'r') && promotedPiece != sameColour(boardPiece, 'q'))
        {
            cout<<"Invalid piece for promotion (Tip: The input is case-sensitive)"<<endl;
            cin>>promotedPiece;
        }
        this->board[chosen.endX][chosen.endY].setPiece(promotedPiece);
    }
    this->updateBoard();
}

void Board::undoMove()
{
    Move &chosen = this->recordedMoves.back();
    this->board[chosen.startX][chosen.startY] = this->board[chosen.endX][chosen.endY];
    this->board[chosen.endX][chosen.endY] = Piece::make_piece(this);
    this->moved[chosen.startX][chosen.startY] = false;
    if (chosen.castle)
    {
        this->board[chosen.rookStartX][chosen.rookStartY] = this->board[chosen.rookEndX][chosen.rookEndY];
        this->board[chosen.rookEndX][chosen.rookEndY] = Piece::make_piece(this);
        this->moved[chosen.rookStartX][chosen.rookStartY] = false;
    }
    else if (chosen.pawnSpecial == 1)
        this->board[chosen.startX][chosen.endY] = Piece::make_piece(!this->board[chosen.startX][chosen.startY].getColour(), chosen.startX, chosen.startY, 'p', this);
    else
    {
        this->board[chosen.endX][chosen.endY].setPiece(chosen.takenPiece);
        if (chosen.pawnSpecial == 2)
            this->board[chosen.startX][chosen.startY].setPiece(sameColour(this->board[chosen.startX][chosen.startY].getPiece(), 'p'));
    }
    this->recordedMoves.pop_back();
    this->updateBoard();
}