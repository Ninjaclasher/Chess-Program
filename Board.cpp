/**********************************************************************************
*      Board.cpp                                                                  *
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

//Constructors
Board::Board()
{
    initialize();
    char start [8][8] = {{'r','n','b','q','k','b','n','r'},
                         {'p','p','p','p','p','p','p','p'},
                         {'.','.','.','.','.','.','.','.'},
                         {'.','.','.','.','.','.','.','.'},
                         {'.','.','.','.','.','.','.','.'},
                         {'.','.','.','.','.','.','.','.'},
                         {'P','P','P','P','P','P','P','P'},
                         {'R','N','B','Q','K','B','N','R'}};
    for (auto& x : range)
        for (auto& y : range)
            board[x][y] = Piece::make_piece(Piece::getColour(start[x][y]), x, y, tolower(start[x][y]), this);
}

Board::Board(char start[8][8])
{
    initialize();
    for (auto& x : range)
        for (auto& y : range)
            board[x][y] = Piece::make_piece(Piece::getColour(start[x][y]), x, y, tolower(start[x][y]), this);
}

//Public member functions
pair<int,bool> Board::nextTurn(bool &playerTurn)
{
    currentOptions.clear();
    displayBoard(playerTurn);
    int tmpCheck = checkmate(playerTurn);
    return make_pair(tmpCheck, max(tmpCheck, static_cast<int>(stalemate(playerTurn))));
}

void Board::displayBoard(const bool side) const
{
    int startX = (side ? 11 : 0), endX = (side ? -1 : 12);
    cout<<endl<<endl;
    for (int x = startX; x != endX; x += (endX-startX)/abs(endX-startX))
    {
        if (x == 1 || x == 10)
            cout<<" ~+~~~~~~~~~~~~~~~~~+~"<<endl;
        else if (x == 0 || x == 11)
            cout<<" X| 0 1 2 3 4 5 6 7 |X"<<endl;
        else
        {
            cout<<" "<<x-2<<"| ";
            for (auto& y : range)
                cout<<board[x-2][y]<<" ";
            cout<<"|"<<x-2<<endl;
        }
    }
    cout<<endl;
}

bool Board::displayPieceMoves(const char piece)
{
    bool movesAvailable = (find_if(currentOptions.begin(), currentOptions.end(), [piece, this](auto& x) {return board[x.startX][x.startY].getPiece() == piece;}) != currentOptions.end());
    if (!currentOptions.empty() && movesAvailable)
    {
        for (auto& y : currentOptions)
            if (board[y.startX][y.startY].getPiece() == piece)
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
            for (unsigned int x = 0; x < currentOptions.size() && !valid; x++)
            {
                if (board[currentOptions[x].startX][currentOptions[x].startY].getPiece() == piece && currentOptions[x].startX == sX && currentOptions[x].startY == sY && currentOptions[x].endX == eX && currentOptions[x].endY == eY)
                    valid = true, pointer = x;
                else if (x == currentOptions.size() - 1)
                    cout<<"Invalid position. Please try again."<<endl;
            }
        }
        recordedMoves.push_back(currentOptions[pointer]);
        applyMove(recordedMoves.back());
    }
    else
    {
        cout<<"No valid moves available for this piece."<<endl;
        return false;
    }
    return true;
}

void Board::getPieceMoves(const char piece)
{
    vector<pair<int,int>> pos = returnPos(piece);
    for (auto& z : pos)
        board[z.first][z.second].getMoves(currentOptions);
}

bool Board::undoMove()
{
    if (recordedMoves.empty()) return false;
    Move &chosen = recordedMoves.back();
    board[chosen.startX][chosen.startY] = board[chosen.endX][chosen.endY];
    board[chosen.endX][chosen.endY] = Piece::make_piece(this);
    moved[chosen.startX][chosen.startY] = false;
    if (chosen.castle)
    {
        board[chosen.rookStartX][chosen.rookStartY] = board[chosen.rookEndX][chosen.rookEndY];
        board[chosen.rookEndX][chosen.rookEndY] = Piece::make_piece(this);
        moved[chosen.rookStartX][chosen.rookStartY] = false;
    }
    else if (chosen.pawnSpecial == 1)
        board[chosen.startX][chosen.endY] = Piece::make_piece(!board[chosen.startX][chosen.startY].getColour(), chosen.startX, chosen.startY, 'p', this);
    else
    {
        board[chosen.endX][chosen.endY].setPiece(chosen.takenPiece);
        if (chosen.pawnSpecial == 2)
            board[chosen.startX][chosen.startY].setPiece(Piece::sameColour(board[chosen.startX][chosen.startY].getPiece(), 'p'));
    }
    recordedMoves.pop_back();
    updateBoard();
    return true;
}


void Board::Save_To_File(const string fileName)
{
    ofstream savedFile (fileName);
    savedFile<<"CONSOLE CHESS ---------------------------------------- CONSOLE CHESS"<<endl;
    for (auto& x : range)
    {
        for (auto& y : range)
        {
            Piece &tmp = board[x][y];
            savedFile<<tmp.getPiece()<<" "<<tmp.getColour()<<" "<<tmp.getPos().first<<" "<<tmp.getPos().second<<" "<<moved[x][y]<<endl;
        }
    }
    savedFile<<"MOVE SEQUENCE ---------------------------------------- MOVE SEQUENCE"<<endl;
    for (auto &x : recordedMoves)
    {
        savedFile<<x.startX<<" "<<x.startY<<" "<<x.endX<<" "<<x.endY<<" "<<x.takenPiece<<" "<<x.pawnSpecial<<" "<<x.castle;
        if (x.castle)
            savedFile<<" "<<x.rookStartX<<" "<<x.rookStartY<<" "<<x.rookEndX<<" "<<x.rookEndY;
        savedFile<<endl;
    }
    savedFile<<"END OF FILE -------------------------------------------- END OF FILE"<<endl;
}

bool Board::Load_From_File(const string fileName, bool &playerTurn)
{
    ifstream loadedFile(fileName);
    if (!static_cast<bool>(loadedFile)) return false;
    string line;
    getline(loadedFile, line);
    if (line != "CONSOLE CHESS ---------------------------------------- CONSOLE CHESS") return false;
    for (auto& x : range)
    {
        for (auto& y : range)
        {
            char piece;
            int colour, posX, posY;
            bool moved;
            loadedFile>>piece>>colour>>posX>>posY>>moved;
            board[x][y] = Piece::make_piece(colour, posX, posY, piece, this);
            this->moved[x][y] = moved;
        }
    }
    getline(loadedFile, line);
    recordedMoves.clear();
    getline(loadedFile, line);
    while(line != "END OF FILE -------------------------------------------- END OF FILE")
    {
        int sX = line[0] - '0', sY = line[2] - '0', eX = line[4] - '0', eY = line[6] - '0', pawn = line[10] - '0';
        char takenPiece = line[8];
        bool castle = line[12] - '0';
        if (castle)
        {
            int rsX = line[14] - '0', rsY = line[16] - '0', reX = line[18] - '0', reY = line[20] - '0';
            recordedMoves.push_back(Move(sX, sY, eX, eY, rsX, rsY, reX, reY, castle));
        }
        else
            recordedMoves.push_back(Move(sX, sY, eX, eY, takenPiece, pawn));
        getline(loadedFile, line);        
    }
    playerTurn = !(recordedMoves.size() % 2);
    return true;
}

//Private member functions
void Board::initialize()
{
#ifdef __unix__
    system("printf '\e[8;30;75t'");
    system("clear");
#elif _WIN32
    system("MODE 80,25");
    system("cls");
#endif
    currentOptions.clear(), recordedMoves.clear();
}

vector<pair<int,int>> Board::returnPos (char piece) const
{
    vector<pair<int,int>> pos;
    //iterate through all the positions on the board to find all occurences of the piece selected
    for (auto& x : range)
        for (auto& y : range)
            if (board[x][y].getPiece() == piece)
                pos.push_back(make_pair(x,y));
    return pos;
}

void Board::applyMove(Move& chosen)
{
    board[chosen.endX][chosen.endY] = board[chosen.startX][chosen.startY];
    board[chosen.startX][chosen.startY] = Piece::make_piece(this);
    moved[chosen.startX][chosen.startY] = true;
    if (chosen.castle)
    {
        board[chosen.rookEndX][chosen.rookEndY] = board[chosen.rookStartX][chosen.rookStartY];
        board[chosen.rookStartX][chosen.rookStartY] = Piece::make_piece(this);
        moved[chosen.rookStartX][chosen.rookStartY] = true;
    }
    else if (chosen.pawnSpecial == 1)
        board[chosen.endX + (board[chosen.endX][chosen.endY].getPiece() == 'p' ? -1 : 1)][chosen.endY] = Piece::make_piece(this);
    else if (chosen.pawnSpecial == 2)
    {
        char promotedPiece, boardPiece = board[chosen.endX][chosen.endY].getPiece();
        cout <<"Promote piece to: "<<Piece::sameColour(boardPiece, 'n')<<" "<<Piece::sameColour(boardPiece, 'b')<<" "<<Piece::sameColour(boardPiece, 'r')<<" "<<Piece::sameColour(boardPiece, 'q')<<endl;
        cin>>promotedPiece;
        while (promotedPiece != Piece::sameColour(boardPiece, 'n') && promotedPiece != Piece::sameColour(boardPiece, 'b') && promotedPiece != Piece::sameColour(boardPiece, 'r') && promotedPiece != Piece::sameColour(boardPiece, 'q'))
        {
            cout<<"Invalid piece for promotion (Tip: The input is case-sensitive)"<<endl;
            cin>>promotedPiece;
        }
        board[chosen.endX][chosen.endY].setPiece(promotedPiece);
    }
    updateBoard();
}

bool Board::stalemate (const bool colour)
{
    vector<Move> tmp;
    //iterate through all positions on the board and see if there are any pieces that can move
    //if there are none, then it is stalemate
    for (unsigned int x = 0; x < 8 && tmp.empty(); x++)
        for (auto& y : range)
            if (board[x][y].getColour() == colour && board[x][y].getPiece() != '.')
                board[x][y].getMoves(tmp);
    return tmp.empty();
}

// -1 = appended valid moves, 0 = not in check, 1 = checkmate
int Board::checkmate (bool side)
{
    //checks if there is even a king on the board
    vector<pair<int,int>> kingPos = returnPos(side ? 'k' : 'K');
    if (kingPos.empty()) return 1;
    //checks if the king is in check and by which pieces
    vector<pair<int,int>> checker = board[kingPos[0].first][kingPos[0].second].inCheck(board[kingPos[0].first][kingPos[0].second].getPiece());
    if (checker.empty()) return 0;

    board[kingPos[0].first][kingPos[0].second].getMoves(currentOptions);
    //if the king is checked by more than one piece, then only the king can move
    //if the king cannot move, it is checkmated
    if (checker.size() > 1) return (currentOptions.empty() ? 1 : -1);
    //if the king is only checked by one piece, iterate through all positions between the king and the piece checking the king
    //check if any ally piece can block the line of sight or take the piece checking the king
    else if (checker.size() == 1)
    {
        unsigned int posX = checker[0].first, posY = checker[0].second;
        int distX = kingPos[0].first - posX, distY = kingPos[0].second - posY;
        distX /= distX ? abs(distX) : 1;
        distY /= distY ? abs(distY) : 1;
        for (unsigned int i = 0, x = posX, y = posY; i < 8 && inBound(x, y) && (x != kingPos[0].first || y != kingPos[0].second) && (tolower(board[posX][posY].getPiece()) != 'n' || i == 0); i++, x += distX, y += distY)
        {
            vector<pair<int,int>> tmpPos = board[x][y].inCheck(board[posX][posY].getPiece());
            //iterate through all pieces that can move to the current position of (x, y)
            for (auto& j : tmpPos)
                if (tolower(board[j.first][j.second].getPiece()) != 'p' || (posX == x && posY == y)) //if the piece is not a pawn, or if this is the first iteration, add it to currentOptions as a possible move
                    currentOptions.push_back(Move(j.first, j.second, x, y, board[x][y].getPiece()));
            if ((side && board[x-1][y].getPiece() == 'p') || (!side && board[x+1][y].getPiece() == 'P'))
                currentOptions.push_back(Move(x + (side ? -1 : 1), y, x, y, board[x][y].getPiece()));
        }
        return (currentOptions.empty() ? 1 : -1);
    }
    return 0;
}

void Board::updateBoard()
{
    for (auto& x : range)
        for (auto& y : range)
            board[x][y].setPos(x, y);
}
