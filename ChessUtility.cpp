#pragma once
#include <algorithm>
#include <cassert>
#include <bitset>
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::make_pair;
using std::unordered_map;
using std::bitset;


unordered_map<char,char> oppositePiece = {{'r', 'R'}, {'R', 'r'},
                                          {'n', 'N'}, {'N', 'n'},
                                          {'b', 'B'}, {'B', 'b'},
                                          {'q', 'Q'}, {'Q', 'q'},
                                          {'k', 'K'}, {'K', 'k'},
                                          {'p', 'P'}, {'P', 'p'},
                                          {'.','.'}};

pair<int,int> knightXY[8] = {std::make_pair(2, 1), std::make_pair(2, -1), std::make_pair(-2, 1), std::make_pair(-2, -1), 
                                  std::make_pair(1, 2), std::make_pair(1, -2), std::make_pair(-1, 2), std::make_pair(-1, -2)};

pair<int,int> QBRKXY[8] = {std::make_pair(1, 0), std::make_pair(-1, 0), std::make_pair(0, 1), std::make_pair(0, -1), 
                                std::make_pair(1, 1), std::make_pair(1, -1), std::make_pair(-1, 1), std::make_pair(-1, -1)};


template <typename T>
void getNum (T &x)
{
    while(!(cin >> x))
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        cout << "Please enter a number: ";
    }
}

inline bool inBound (int posX, int posY) {return (posX < 8 && posX >= 0 && posY < 8 && posY >= 0);}

inline int getColour (char piece) {return (piece == '.' ? 2 : (piece == 'q' || piece == 'r' || piece == 'b' || piece == 'n' || piece == 'k' || piece == 'p' ? 1 : 0));}

inline char sameColour(char basePiece, char piece) {return getColour(basePiece) == getColour(piece) ? piece : oppositePiece[piece];}
