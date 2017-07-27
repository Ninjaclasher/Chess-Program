/*TODO LIST
 *Add 3-time repetition
 *Allow game state saving and reloading
 *??Add option to end game anytime
 *??Add option to choose another piece to move
 *Check for bugs
 */
#include <iostream>

#include "Chess.h"

using namespace std;

int main()
{
    Board game;
    char piece;
    bool playerTurn = true, loop = true;;
    int checkmate, exitReason = 255; // 0 = checkmate, 1 = stalemate, 2 = draw, 3 = resign, 4 = exit, 255 = unknown
    vector<Move> options;
    while (exitReason == 255)
    {
        options.clear();
        playerTurn ^= loop;
        game.displayBoard(playerTurn);
        checkmate = game.checkmate(playerTurn ? 'k' : 'K', options);
        if (checkmate == 0 || game.stalemate(playerTurn))
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
                    game.getPieceMoves(piece, options);
                loop = game.displayPieceMoves(options);
            break;
            case 'u':
                if (game.recordedMoves.empty())
                    cout<<"No moves have been done yet."<<endl;
                else
                {                
                    cout<<"Are you sure you want to undo?"<<endl;
                    cin>>piece;
                    if (tolower(piece) == 'y')
                    {
                        game.undoMove();
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