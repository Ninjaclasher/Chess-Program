/**********************************************************************************
*      Move.cpp                                                                   *
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

Move::Move(int sX, int sY, int eX, int eY, char pp)
{
    startX = sX, startY = sY, endX = eX, endY = eY;
    takenPiece = pp;
}

Move::Move(int sX, int sY, int eX, int eY, char pp, int s)
{
    startX = sX, startY = sY, endX = eX, endY = eY;
    takenPiece = pp, pawnSpecial = s;
}

Move::Move(int sX, int sY, int eX, int eY, int rsX, int rsY, int reX, int reY, bool c)
{
    startX = sX, startY = sY, endX = eX, endY = eY;
    rookStartX = rsX, rookStartY = rsY, rookEndX = reX, rookEndY = reY;
    castle = c;
}