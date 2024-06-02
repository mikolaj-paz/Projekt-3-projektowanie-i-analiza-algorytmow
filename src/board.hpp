#ifndef BOARD_HPP
#define BOARD_HPP

#include <string>
#include <map>

#include "pieces.hpp"
#include "move.hpp"

class Board
{
    public:
        Board();

        ~Board()
        { 
            delete[] squares;
            delete[] bitboards;
        }

        const int* const get() const
            { return squares; }

        const Move& getLastMove() const
            { return lastMove; }

        const bool* getCastleK() const
            { return castleK; }

        const bool* getCastleQ() const
            { return castleQ; }

        void update(const Move move);

        const int toMove() const
            { return blackToMove ? Piece.black : Piece.white; }

        void updateBitboards(const Move& move);

        void loadFEN(const std::string FENstring = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
        unsigned long long whiteKingSquare;
        unsigned long long blackKingSquare;   
    private:
        int* squares;
        bool blackToMove;

        bool castleK[2]; // roszada krolewska
        bool castleQ[2]; // roszada hetmanska


        Move lastMove;

        unsigned long long* bitboards;
};

#endif