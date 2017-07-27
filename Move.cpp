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