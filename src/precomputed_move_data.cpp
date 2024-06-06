#include "precomputed_move_data.hpp"

unsigned long long PrecomputedMoveData::whitePawnAttackBitboards[64] = { 0ULL };
unsigned long long PrecomputedMoveData::blackPawnAttackBitboards[64] = { 0ULL };

unsigned long long PrecomputedMoveData::kingAttackBitboards[64] = { 0ULL };

unsigned long long PrecomputedMoveData::knightAttackBitboards[64] = { 0ULL };

void PrecomputedMoveData::init()
{
    int blackOffset = 3;
    const int pawnOffsets[6] = { 7, 8, 9, -9, -8, -7 };
    const int directionOffsets[8] = { 8, -8, -1, 1, 7, -7, 9, -9 };
    const int knightOffsets[8] = { 15, 17, -17, -15, 6, -10, 10, -6 };

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

    int margin[8];
    for (int square = 0; square < 64; square++)
    {
        margin[BOT] = square / 8;
        margin[TOP] = 7 - margin[BOT];
        margin[LEFT] = square % 8;
        margin[RIGHT] = 7 - margin[LEFT];

        margin[TOP_LEFT] = margin[TOP] < margin[LEFT] ? margin[TOP] : margin[LEFT];
        margin[TOP_RIGHT] = margin[TOP] < margin[RIGHT] ? margin[TOP] : margin[RIGHT];
        margin[BOT_LEFT] = margin[BOT] < margin[LEFT] ? margin[BOT] : margin[LEFT];
        margin[BOT_RIGHT] = margin[BOT] < margin[RIGHT] ? margin[BOT] : margin[RIGHT];

        // =========================================================================================
        //  Kalkulacja rochow krola

        int squareIndex;
        for (int j = 0; j < 8; j++)
        {
            if (margin[j] > 0)
            {
                squareIndex = square + directionOffsets[j];
                kingAttackBitboards[square] |= 1ULL << squareIndex;
            }
        }
        // =========================================================================================
        
        // =========================================================================================
        // Kalkulacja rochow skoczka

        int j = 0;
        for (int k = 0; k < 2; k++)
        {   
            for (int l = 2; l < 4; l++)
            {
                if (margin[k] > 1 && margin[l] > 0)
                {
                    squareIndex = square + knightOffsets[j];
                    knightAttackBitboards[square] |= 1ULL << squareIndex;
                }
                j++;
            }
        }

        for (int k = 2; k < 4; k++)
        {   
            for (int l = 0; l < 2; l++)
            {
                if (margin[k] > 1 && margin[l] > 0)
                {
                    squareIndex = square + knightOffsets[j];
                    knightAttackBitboards[square] |= 1ULL << squareIndex;
                }
                j++;
            }
        }
        // =========================================================================================

        // =========================================================================================
        //  Kalkulacja atakow pionka
        
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
        // =========================================================================================
    }
}