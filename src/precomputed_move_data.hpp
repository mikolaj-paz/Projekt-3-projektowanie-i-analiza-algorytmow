#ifndef PRECOMPUTED_MOVE_DATA_HPP
#define PRECOMPUTED_MOVE_DATA_HPP

#include "pieces.hpp"

class PrecomputedMoveData
{
    public:
        PrecomputedMoveData() = delete;

        static void init();

        static unsigned long long whitePawnAttacks(int square)
            { return whitePawnAttackBitboards[square]; }

        static unsigned long long blackPawnAttacks(int square)
            { return blackPawnAttackBitboards[square]; }

    private:
        static unsigned long long whitePawnAttackBitboards[64];
        static unsigned long long blackPawnAttackBitboards[64];
};

#endif