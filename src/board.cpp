#include "board.hpp"

Board::Board(): lastMove{Move(0,0,0)}
{
    squares = new int[64];
    bitboards = new unsigned long long[64];
    for (int i = 0; i < 64; i++)
    {
        squares[i] = 0;
        bitboards[i] = 0;
    }

    loadFEN();
}

void Board::update(const Move move)
{
    squares[move.getTarget()] = squares[move.getOrigin()];
    squares[move.getOrigin()] = 0;

    if ((move.getPiece() & Piece.typeMask) == Piece.pawn)
    {
        if (blackToMove)
        {
            if (move.getTarget() / 8 == 0)
                squares[move.getTarget()] = Piece.black | Piece.queen;
            else if (move.enPassant())
                squares[move.getTarget() + 8] = 0;
        }
        else 
        {
            if (move.getTarget() / 8 == 7)
                squares[move.getTarget()] = Piece.white | Piece.queen;
            else if (move.enPassant())
                squares[move.getTarget() - 8] = 0;
        }
    }

    lastMove = move;
    blackToMove = !blackToMove;
}

void Board::updateBitboards(const Move& move)
{
    int pieceToMove = squares[move.getOrigin()];
    int pieceToCapture = squares[move.getTarget()];

    bitboards[pieceToMove] ^= 1ULL << move.getOrigin() | 1ULL << move.getTarget();
    if (pieceToCapture != Piece.none)
    {
        bitboards[pieceToCapture] ^= 1ULL << move.getTarget();
    }
}

void Board::loadFEN(const std::string FENstring)
{
    std::map<char,int> dictionary
    {
        { 'b', Piece.bishop },
        { 'k', Piece.king },
        { 'n', Piece.knight },
        { 'p', Piece.pawn },
        { 'q', Piece.queen },
        { 'r', Piece.rook }
    };

    int i = 0, file = 0, rank = 7;
    char c = FENstring[i];
    while (c != ' ')
    {
        if (c == '/')
        {
            file = 0;
            --rank;
        }
        else
        {
            if (c >= '0' && c <= '9')
                file += (c - '0');
            else
            {
                int color;
                int type;
                if (c > 'Z')
                {
                    color = Piece.black;
                    type = dictionary[c];
                }
                else 
                {
                    color = Piece.white;
                    type = dictionary[c + 32];
                }
                squares[rank * 8 + file] = type | color;
                bitboards[type | color] |= 1ULL << rank * 8 + file;
                file++;
            }
        }

        c = FENstring[++i];
    }

    // Informacje dodatkowe (nieobslugiwane)
    blackToMove = false;
}