#include "precomputed_move_data.hpp"

unsigned long PrecomputedMoveData::knightMovesBitboards[64] = { 0 };
unsigned long PrecomputedMoveData::bishopMovesBitboards[64] = { 0 };
unsigned long PrecomputedMoveData::rookMovesBitboards[64] = { 0 };
unsigned long PrecomputedMoveData::queenMovesBitboards[64] = { 0 };
unsigned long PrecomputedMoveData::whitePawnMovesBitboards[64] = { 0 };
unsigned long PrecomputedMoveData::whitePawnAttackBitboards[64] = { 0 };
unsigned long PrecomputedMoveData::blackPawnMovesBitboards[64] = { 0 };
unsigned long PrecomputedMoveData::blackPawnAttackBitboards[64] = { 0 };

void PrecomputedMoveData::init()
{

    const int directionOffsets[9] = { 8, -8, -1, 1, 7, -7, 9, -9 };

    int topDistOrigin = 7;
    int botDistOrigin = 0;
    int rightDistOrigin = 7;
    int leftDistOrigin = 0;

    for (int i = 0; i < 64; i++)
    {
        // orthagonal
        // for (int j = 1; j <= topDistOrigin; j++)
        //     rookMovesBitboards[i] |= 1UL << ( i + j * directionOffsets[0] );

        // for (int j = 1; j <= botDistOrigin; j++)
        //     rookMovesBitboards[i] |= 1UL << ( i + j * directionOffsets[1] );

        for (int j = 1; j <= leftDistOrigin; j++)
            rookMovesBitboards[i] |= 1UL << ( i + j * directionOffsets[2] );

        for (int j = 1; j <= rightDistOrigin; j++)
            rookMovesBitboards[i] |= 1UL << ( i + j * directionOffsets[3] );

        // diagonal
        // for (int j = 1; j <= topDistOrigin && j <= leftDistOrigin; j++)
        //     bishopMovesBitboards[i] |= 1UL << ( i + j * directionOffsets[4] );

        // for (int j = 1; j <= botDistOrigin && j <= rightDistOrigin; j++)
        //     bishopMovesBitboards[i] |= 1UL << ( i + j * directionOffsets[5] );

        // for (int j = 1; j <= topDistOrigin && j <= rightDistOrigin; j++)
        //     bishopMovesBitboards[i] |= 1UL << ( i + j * directionOffsets[6] );

        // for (int j = 1; j <= botDistOrigin && j <= leftDistOrigin; j++)
        //     bishopMovesBitboards[i] |= 1UL << ( i + j * directionOffsets[7] );

        // queenMovesBitboards[i] = rookMovesBitboards[i] | bishopMovesBitboards[i];

        leftDistOrigin++;
        rightDistOrigin--;
        
        if (i % 8 == 7)
        {
            topDistOrigin--;
            botDistOrigin++;
            leftDistOrigin = 0;
            rightDistOrigin = 7;
        }
    }
}