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
            if ((piece & Piece.typeMask) == Piece.king && abs(targetIndex - originIndex) == 2)
                isCastle = true;
            else if ((piece & Piece.typeMask) == Piece.pawn && (abs(targetIndex - originIndex) % 8) != 0)
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
        
        const bool& castle() const
            { return isCastle; }

    private:
        int originSquare;
        int targetSquare;

        bool isEnPassant = false;
        bool isCastle = false;

        int piece;
};

#endif