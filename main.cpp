/*TODO LIST
 *Add 3-time repetition
 *Allow game state saving and reloading
 *??Add option to end game anytime
 *??Add option to choose another piece to move
 *Check for bugs
 */
#include "Chess.h"

using namespace std;

void saveGame(Board &game)
{
    string fileName;
    cout<<"Enter a file name to save the game to (Enter \"cancel\" to cancel): ";
    clearInput();
    getline(cin, fileName);
    string tmp = fileName;
    for (auto& x : tmp) x = tolower(x);
    if (tmp == "cancel") return;
    game.Save_To_File(fileName);
    cout<<"Saved game successfully to "<<fileName<<endl;
}

void loadGame(Board &game, bool &playerTurn)
{
    string fileName;
    cout<<"Enter a file name to load game from (Enter \"cancel\" to cancel): ";
    clearInput();
    getline(cin, fileName);
    while(!game.Load_From_File(fileName, playerTurn))
    {
        string tmp = fileName;
        for (auto& x : tmp) x = tolower(x);
        if (tmp == "cancel") return;
        cout<<"Invalid file name/File is not the correct format. Please enter another file name: ";
        getline(cin, fileName);
    }
    cout<<"Loaded game successfully from "<<fileName<<endl;
}

void undo(Board &game, bool &loop)
{
    char input;
    cout<<"Are you sure you want to undo?"<<endl;
    cin>>input;
    if (tolower(input) == 'y')
    {
        if (game.undoMove())
        {
            loop = true;
            cout<<"Move undone."<<endl;
        }
        else
            cout<<"No moves have been done yet."<<endl;
    }
}

void help()
{
    cout<<"+~~~~~~~~~~~~~~~~~~~~~+"<<endl;
    cout<<"|\"u\" - undo a move.   |"<<endl;
    cout<<"|\"d\" - offer draw.    |"<<endl;
    cout<<"|\"g\" - give up.       |"<<endl;
    cout<<"|\"r\" - move a Rook.   |"<<endl;
    cout<<"|\"b\" - move a Bishop. |"<<endl;
    cout<<"|\"n\" - move a Knight. |"<<endl;
    cout<<"|\"q\" - move a Queen.  |"<<endl;
    cout<<"|\"k\" - move the King. |"<<endl;
    cout<<"|\"p\" - move a Pawn.   |"<<endl;
    cout<<"|\"s\" - save game.     |"<<endl;
    cout<<"|\"l\" - load game.     |"<<endl;
    cout<<"|\"e\" - exit game.     |"<<endl;
    cout<<"+~~~~~~~~~~~~~~~~~~~~~+"<<endl;
    cout<<" Press Enter to return"<<endl;
    clearInput();
    clearInput();    
}

pair<int,int> startChessGame(Board &game)
{
    char piece;
    bool playerTurn = true, loop = true;;
    while (true)
    {
        playerTurn ^= loop;
        int checkState = 0;
        bool gameState = false;
        tie(checkState, gameState) = game.nextTurn(playerTurn);
        if (gameState) return make_pair((checkState == 1 ? 0 : 1), playerTurn);
        cout<<(playerTurn ? "Black" : "White")<<"'s turn. Enter a piece to move. Enter \"h\" for help."<<endl;
        cin>>piece;
        piece = (playerTurn) ? tolower(piece) : toupper(piece);
        loop = false;
        switch(tolower(piece))
        {
            case 'r': case 'b': case 'n': case 'q': case 'k': case 'p':
                if (checkState == 0) game.getPieceMoves(piece);
                loop = game.displayPieceMoves(piece);
            break;
            case 'u':
                undo(game, loop);
            break;
            case 'd':
                cout<<(playerTurn ? "Black": "White")<<" wants to draw. "<<(playerTurn ? "White" : "Black")<<" - do you want to draw?"<<endl;
                cin>>piece;
                if (tolower(piece) == 'y') return make_pair(2, playerTurn);
            break;
            case 'g':
                cout<<"Are you sure you want to give up?"<<endl;
                cin>>piece;
                if (tolower(piece) == 'y') return make_pair(3, playerTurn);
            break;
            case 's':
                saveGame(game);
            break;
            case 'l':
                loadGame(game, playerTurn);
            break;
            case 'h':
                help();
            break;
            case 'e':
                cout<<"Are you sure you want to exit game?"<<endl;
                cin>>piece;
                if (tolower(piece) == 'y')
                {
                    cout<<"Would you like the save the game?"<<endl;
                    cin>>piece;
                    if (tolower(piece) == 'y')
                        saveGame(game);
                    else
                        return make_pair(4, playerTurn);
                }
            break;
            default:
                cout<<"Invalid piece/command entered."<<endl;
            break;
        }
    }
    return make_pair(255, 0);
}

void initialize()
{
#ifdef __unix__
    system("printf '\e[8;30;75t'");
    system("clear");
#elif _WIN32
    system("MODE 80,25");
    system("cls");
#endif    
}

int main()
{
    initialize();
    Board game;
    pair<int,bool> state = startChessGame(game);  // (first) 0 = checkmate, 1 = stalemate, 2 = draw, 3 = resign, 4 = exit, 255 = unknown (second) 0 = white 1 = black
    switch(state.first)
    {
        case 0:
            cout<<(state.second ? "Black" : "White")<<" is checkmated!"<<endl;
        break;
        case 1:
            cout<<"Stalemate! No one wins!"<<endl;
        break;
        case 2:
            cout<<"A draw has been agreed! No one wins!"<<endl;
        break;
        case 3:
            cout<<(state.second ? "Black" : "White")<<" resigns!"<<endl;
        break;
        case 4:
            cout<<(state.second ? "Black" : "White")<<" exited the game!"<<endl;
        break;
    }
    return 0;
}
