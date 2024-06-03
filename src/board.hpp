#ifndef BOARD_HPP
#define BOARD_HPP

#include <string>
#include <map>
#include <algorithm>

#include "pieces.hpp"
#include "move.hpp"

class Board
{
    public:
        Board();

        Board(const Board& other):
            blackToMove{other.blackToMove}, 
            whiteKingSquare{other.whiteKingSquare}, blackKingSquare{other.blackKingSquare},
            whitePiecesBitboard{other.whitePiecesBitboard}, blackPiecesBitboard{other.blackPiecesBitboard},
            lastMove{other.lastMove}
        {
            for (int i = 0; i < 64; i++)
                squares[i] = other.squares[i];

            castleK[0] = other.castleK[0];
            castleQ[0] = other.castleQ[0];
            castleK[1] = other.castleK[1];
            castleQ[1] = other.castleQ[1];
        }

        Board& operator=(const Board& other);

        const int* const get() const
            { return squares; }

        const Move& getLastMove() const
            { return lastMove; }

        const bool* getCastleK() const
            { return castleK; }

        const bool* getCastleQ() const
            { return castleQ; }

        const unsigned long long& getWhiteKingSquare() const
            { return whiteKingSquare; }

        const unsigned long long& getBlackKingSquare() const
            { return blackKingSquare; }

        const unsigned long long& getWhiteBitboard() const
            { return whitePiecesBitboard; }

        const unsigned long long& getBlackBitboard() const
            { return blackPiecesBitboard; }

        void update(const Move move);

        const int toMove() const
            { return blackToMove ? Piece.black : Piece.white; }

        void updateBitboards(const Move& move);

        void loadFEN(const std::string FENstring = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
           
        unsigned long long whiteKingSquare;
        unsigned long long blackKingSquare;
    private:
        int squares[64];
        bool blackToMove;

        bool castleK[2]; // roszada krolewska
        bool castleQ[2]; // roszada hetmanska

        unsigned long long whitePiecesBitboard;
        unsigned long long blackPiecesBitboard;

        Move lastMove;
};

#endif