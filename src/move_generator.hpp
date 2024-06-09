#ifndef MOVE_GENERATOR_HPP
#define MOVE_GENERATOR_HPP

#include <vector>
#include <algorithm>
#include <math.h>
#include <random>
#include <chrono>

#include "precomputed_move_data.hpp"
#include "pieces.hpp"
#include "board.hpp"

class MoveGenerator
{
    public:
        MoveGenerator() = delete;

        static const GameState getBoardState(const Board* board);

        static std::vector<Move> getAllLegalMoves(const Board* board, bool onlyCaptures = false);

        static std::vector<Move> getLegalMoves(const Board* board, int square, unsigned long long* bitboard, bool onlyCaptures = false);

        static std::vector<Move> getMoves(const Board* board, int square, unsigned long long* bitboard, bool friendlyFire = false, bool onlyCaptures = false);

        static unsigned long long attackedSquares(const Board* board, const int& friendlyPieceColor);

        static bool isWhiteKingInCheck(const Board* board)
        {
            if (attackedSquares(board, Piece.black) & board->getWhiteKingSquare())
                return true;
            return false;
        }

        static bool isBlackKingInCheck(const Board* board)
        {
            if (attackedSquares(board, Piece.white) & board->getBlackKingSquare())
                return true;
            return false;
        }
        
        static bool isInsufficientMaterial(const Board* board);

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

        static unsigned long long generatePawn(const int* board, const int& square, const int& friendlyPieceColor);
        static unsigned long long generatePawnAttacks(const Board* board, const int& square, const int& friendlyPieceColor);
};

#endif