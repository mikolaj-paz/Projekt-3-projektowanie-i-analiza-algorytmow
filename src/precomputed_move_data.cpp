//https://www.chessprogramming.org/Simplified_Evaluation_Function

#include "precomputed_move_data.hpp"

unsigned long long PrecomputedMoveData::whitePawnAttackBitboards[64] = { 0ULL };
unsigned long long PrecomputedMoveData::blackPawnAttackBitboards[64] = { 0ULL };

unsigned long long PrecomputedMoveData::kingAttackBitboards[64] = { 0ULL };

unsigned long long PrecomputedMoveData::knightAttackBitboards[64] = { 0ULL };

int PrecomputedMoveData::pieceSquareScore[15][64] =
{
    { 0 },

    {   // Bialy pionek
        0,  0,  0,  0,  0,  0,  0,  0,
        5, 10, 10,-20,-20, 10, 10,  5,
        5, -5,-10,  0,  0,-10, -5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5,  5, 10, 25, 25, 10,  5,  5,
        10, 10, 20, 30, 30, 20, 10, 10,
        50, 50, 50, 50, 50, 50, 50, 50,
        0,  0,  0,  0,  0,  0,  0,  0  
    },
    
    {   // Bialy skoczek
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50
    },

    {   // Bialy goniec
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -20,-10,-10,-10,-10,-10,-10,-20
    },

    {   // Biala wieza
        0,  0,  0,  5,  5,  0,  0,  0
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        5, 10, 10, 10, 10, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0
    },

    {   // Biala krolowa
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  5,  0,-10,
        -10,  0,  5,  5,  5,  5,  5,-10,
        -5,  0,  5,  5,  5,  5,  0, 0,
        -5,  0,  5,  5,  5,  5,  0, -5,
        -10,  0,  5,  5,  5,  5,  0,-10,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    },

    {   // Bialy krol, gra srodkowa
        20, 30, 10,  0,  0, 10, 30, 20,
        20, 20,  0,  0,  0,  0, 20, 20,
        -10,-20,-20,-20,-20,-20,-20,-10,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30
    },

    { 0 },
    { 0 },

    {   // Czarny pionek
        0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5,  5, 10, 25, 25, 10,  5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5, -5,-10,  0,  0,-10, -5,  5,
        5, 10, 10,-20,-20, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0
    },

    {   // Czarny skoczek
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50
    },

    {   // Czarny goniec
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-10,-10,-10,-10,-10,-20
    },

    {   // Czarna wieza
        0,  0,  0,  0,  0,  0,  0,  0,
        5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        0,  0,  0,  5,  5,  0,  0,  0
    },

    {   // Czarna krolowa
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
        -5,  0,  5,  5,  5,  5,  0, -5,
         0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    },

    {   // Czarny krol
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -10,-20,-20,-20,-20,-20,-20,-10,
        20, 20,  0,  0,  0,  0, 20, 20,
        20, 30, 10,  0,  0, 10, 30, 20
    }
};

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