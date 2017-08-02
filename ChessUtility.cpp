/**********************************************************************************
*      ChessUtility.cpp                                                           *
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

#pragma once
#include <algorithm>
#include <cassert>
#include <bitset>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using std::array;
using std::bitset;
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::make_pair;
using std::max;
using std::ofstream;
using std::ostream;
using std::pair;
using std::string;
using std::vector;
using std::unordered_map;

array<unsigned int, 8> range {0,1,2,3,4,5,6,7};

unordered_map<char,char> oppositePiece = {{'r', 'R'}, {'R', 'r'},
                                          {'n', 'N'}, {'N', 'n'},
                                          {'b', 'B'}, {'B', 'b'},
                                          {'q', 'Q'}, {'Q', 'q'},
                                          {'k', 'K'}, {'K', 'k'},
                                          {'p', 'P'}, {'P', 'p'},
                                          {'.','.'}};

unordered_map<char,string> pieceDisplay = {{'r', "\u2656"}, {'R', "\u265C"},
                                           {'n', "\u2658"}, {'N', "\u265E"},
                                           {'b', "\u2657"}, {'B', "\u265D"},
                                           {'q', "\u2655"}, {'Q', "\u265B"},
                                           {'k', "\u2654"}, {'K', "\u265A"},
                                           {'p', "\u2659"}, {'P', "\u265F"},
                                           {'.',"."}};

pair<int,int> knightXY[8] = {std::make_pair(2, 1), std::make_pair(2, -1), std::make_pair(-2, 1), std::make_pair(-2, -1), 
                                  std::make_pair(1, 2), std::make_pair(1, -2), std::make_pair(-1, 2), std::make_pair(-1, -2)};

pair<int,int> QBRKXY[8] = {std::make_pair(1, 0), std::make_pair(-1, 0), std::make_pair(0, 1), std::make_pair(0, -1), 
                                std::make_pair(1, 1), std::make_pair(1, -1), std::make_pair(-1, 1), std::make_pair(-1, -1)};

inline void clearInput()
{
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');  
}

template <typename T>
void getNum (T &x)
{
    while(!(cin >> x))
    {
        clearInput();
        cout << "Please enter a number: ";
    }
}

inline bool inBound (int posX, int posY) {return (posX < 8 && posX >= 0 && posY < 8 && posY >= 0);}
