#ifndef PIECES_HPP
#define PIECES_HPP

const struct
{
    const int none = 0;
    const int king = 1;
    const int pawn = 2;
    const int knight = 3;
    const int bishop = 4;
    const int rook = 5;
    const int queen = 6;

    const int white = 8;
    const int black = 16;
} Piece;

#endif