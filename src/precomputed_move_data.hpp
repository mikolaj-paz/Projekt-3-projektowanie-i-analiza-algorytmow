#ifndef PRECOMPUTED_MOVE_DATA_HPP
#define PRECOMPUTED_MOVE_DATA_HPP

#include "pieces.hpp"

class PrecomputedMoveData
{
    public:
        PrecomputedMoveData() = delete;

        static void init();

        static const unsigned long long& whitePawnAttacks(const int& square)
            { return whitePawnAttackBitboards[square]; }

        static const unsigned long long& blackPawnAttacks(const int& square)
            { return blackPawnAttackBitboards[square]; }

        static const unsigned long long& kingAttacks(const int& square)
            { return kingAttackBitboards[square]; }

        static const unsigned long long& knightAttacks(const int& square)
            { return knightAttackBitboards[square]; }

        static const int& squareScore(const int& square, const int& piece)
            { return pieceSquareScore[piece][square]; }

    private:
        static unsigned long long whitePawnAttackBitboards[64];
        static unsigned long long blackPawnAttackBitboards[64];

        static unsigned long long kingAttackBitboards[64];

        static unsigned long long knightAttackBitboards[64];

        static int pieceSquareScore[15][64];
};

#endif