#ifndef PRECOMPUTED_MOVE_DATA_HPP
#define PRECOMPUTED_MOVE_DATA_HPP

class PrecomputedMoveData
{
    public:
        PrecomputedMoveData() = delete;

        static void init();

        static unsigned long kingMoves(int square)
            { return kingMovesBitboards[square]; }

        static unsigned long knightMoves(int square)
            { return knightMovesBitboards[square]; }

        static unsigned long bishopMoves(int square)
            { return bishopMovesBitboards[square]; }

        static unsigned long rookMoves(int square)
            { return rookMovesBitboards[square]; }

        static unsigned long queenMoves(int square)
            { return queenMovesBitboards[square]; }

    private:
        static unsigned long kingMovesBitboards[64];

        static unsigned long knightMovesBitboards[64];

        static unsigned long bishopMovesBitboards[64];
        static unsigned long rookMovesBitboards[64];
        static unsigned long queenMovesBitboards[64];

        static unsigned long whitePawnMovesBitboards[64];
        static unsigned long whitePawnAttackBitboards[64];
        static unsigned long blackPawnMovesBitboards[64];
        static unsigned long blackPawnAttackBitboards[64];
};

#endif