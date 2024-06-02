#include "precomputed_move_data.hpp"

unsigned long long PrecomputedMoveData::whitePawnAttackBitboards[64] = { 0ULL };
unsigned long long PrecomputedMoveData::blackPawnAttackBitboards[64] = { 0ULL };

void PrecomputedMoveData::init()
{
    int blackOffset = 3;
    const int pawnOffsets[6] = { 7, 8, 9, -9, -8, -7 };

    for (int square = 0; square < 64; square++)
    {
        if (square % 8 != 0)
        {
            whitePawnAttackBitboards[square] |= 1ULL << (square + pawnOffsets[0]);
            blackPawnAttackBitboards[square] |= 1ULL << (square + pawnOffsets[blackOffset]);
        }

        if (square % 8 != 7)
        {
            whitePawnAttackBitboards[square] |= 1ULL << (square + pawnOffsets[2]);
            blackPawnAttackBitboards[square] |= 1ULL << (square + pawnOffsets[blackOffset + 2]);
        }
    }
}