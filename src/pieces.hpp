#ifndef PIECES_HPP
#define PIECES_HPP

const struct
{
    const int none = 0;
    const int pawn = 1;
    const int knight = 2;
    const int bishop = 3;
    const int rook = 4;
    const int queen = 5;
    const int king = 6;

    const int white = 0;
    const int black = 8;

    const int colorMask = 0b1000;
    const int typeMask = 0b0111;
} Piece;

#endif