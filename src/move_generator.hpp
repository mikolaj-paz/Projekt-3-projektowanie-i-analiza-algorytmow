#ifndef MOVE_GENERATOR_HPP
#define MOVE_GENERATOR_HPP

#include <vector>
#include <algorithm>
#include <math.h>

#include "precomputed_move_data.hpp"
#include "pieces.hpp"
#include "board.hpp"

class MoveGenerator
{
    public:
        MoveGenerator() = delete;

        static std::vector<Move> getMoves(const Board* board, int square, int piece, unsigned long long* boolBoard);

        static unsigned long long attackedSquares(const Board* board, const int& friendlyPieceColor);

    private:
        static const int directionOffsets[8];
        enum 
        {
            TOP,
            BOT,
            LEFT,
            RIGHT,
            TOP_LEFT,
            BOT_RIGHT,
            TOP_RIGHT,
            BOT_LEFT
        };

        static const int knightOffsets[8];
        static const int pawnOffsets[6];

        static const unsigned long long pawnStart[2];
        static const unsigned long long pawnPromotion[2];

        static unsigned long long generateOrthagonal(const int* board, const int& square, const int& friendlyPieceColor);
        static unsigned long long generateDiagonal(const int* board, const int& square, const int& friendlyPieceColor);

        static unsigned long long generateKing(const Board* board, const int& square, const int& friendlyPieceColor);

        static unsigned long long generateKnight(const int* board, const int& square, const int& friendlyPieceColor);

        static unsigned long long generatePawn(const int* board, const int& square, const int& friendlyPieceColor);
        static unsigned long long generatePawnAttacks(const Board* board, const int& square, const int& friendlyPieceColor);
};

#endif