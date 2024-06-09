#ifndef MOVE_HPP
#define MOVE_HPP

#include <math.h>

#include "pieces.hpp"

class Move
{
    public:
        Move(const int& originIndex, const int& targetIndex, const int& piece, const int capturePiece):
            originSquare{originIndex}, targetSquare{targetIndex}, piece{piece}, capturePiece{capturePiece}
        {
            if ((piece & Piece.typeMask) == Piece.king && abs(targetIndex - originIndex) == 2)
                isCastle = true;
            else if ((piece & Piece.typeMask) == Piece.pawn && (abs(targetIndex - originIndex) % 8) != 0
                     && capturePiece == Piece.none)
                isEnPassant = true;
        }

        const Move operator-() const
            { return Move(targetSquare, originSquare, piece, capturePiece); }

        friend bool operator==(const Move& l, const Move& r)
        {
            return (
                l.originSquare == r.originSquare &&
                l.targetSquare == r.targetSquare &&
                l.piece == r.piece &&
                l.capturePiece == r.capturePiece
            );
        }

        const int& getOrigin() const
            { return originSquare; }

        const int& getTarget() const
            { return targetSquare; }

        const int& getPiece() const
            { return piece; }
            
        const int& getCapturePiece() const
            { return capturePiece; }

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
        int capturePiece;
};

#endif