#ifndef MOVE_HPP
#define MOVE_HPP

#include <math.h>

#include "pieces.hpp"

class Move
{
    public:
        Move(const int& originIndex, const int& targetIndex, const int& piece):
            originSquare{originIndex}, targetSquare{targetIndex}, piece{piece}
        {
            if ((piece & Piece.typeMask) == Piece.pawn && (abs(targetIndex - originIndex) % 8) != 0)
                isEnPassant = true;
        }

        const int& getOrigin() const
            { return originSquare; }

        const int& getTarget() const
            { return targetSquare; }

        const int& getPiece() const
            { return piece; }

        const bool& enPassant() const
            { return isEnPassant; }

    private:
        int originSquare;
        int targetSquare;

        bool isEnPassant = false;

        int piece;
};

#endif