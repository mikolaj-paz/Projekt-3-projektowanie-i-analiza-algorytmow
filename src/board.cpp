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

    // Specjalne ruchy pionka
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

    if (move.getPiece() == (Piece.white | Piece.king))
        whiteKingSquare = 1ULL << move.getTarget();
    else if (move.getPiece() == (Piece.black | Piece.king))
        blackKingSquare = 1ULL << move.getTarget();

    // Roszada
    if (move.castle())
    {
        switch (move.getTarget())
        {
            case 2: // Biala hetmanska
                squares[3] = squares[0];
                squares[0] = 0;
                castleQ[0] = false;
                break;
            case 6: // Biala krolewska
                squares[5] = squares[7];
                squares[7] = 0;
                castleK[0] = false;
                break;
            case 58: // Czarna hetmanska
                squares[59] = squares[56];
                squares[56] = 0;
                castleQ[1] = false;
                break;
            case 62: // Czarna krolewska
                squares[61] = squares[63];
                squares[63] = 0;
                castleK[1] = false;
                break;
            default:
                return;
        };
    }

    // Blokowanie roszady
    if (castleQ[0] && (move.getOrigin() == 0 || move.getTarget() == 0 || move.getOrigin() == 4))
        castleQ[0] = 0;
    else if (castleK[0] && (move.getOrigin() == 7 || move.getTarget() == 7 || move.getOrigin() == 4))
        castleK[0] = 0;
    else if (castleQ[1] && (move.getOrigin() == 56 || move.getTarget() == 56 || move.getOrigin() == 60))
        castleQ[1] = 0;
    else if (castleK[1] && (move.getOrigin() == 63 || move.getTarget() == 63 || move.getOrigin() == 60))
        castleK[1] = 0;

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
                    if (type == Piece.king) blackKingSquare = 1ULL << (rank * 8 + file);
                }
                else 
                {
                    color = Piece.white;
                    type = dictionary[c + 32];
                    if (type == Piece.king) whiteKingSquare = 1ULL << (rank * 8 + file);
                }
                squares[rank * 8 + file] = type | color;
                bitboards[type | color] |= 1ULL << rank * 8 + file;
                file++;
            }
        }

        c = FENstring[++i];
    }

    // ----------------------------------------------------
    // Informacje dodatkowe (nieobslugiwane)
    // ----------------------------------------------------

    blackToMove = false;

    // Biale
    castleK[0] = true;
    castleQ[0] = true;

    // Czarne
    castleK[1] = true;
    castleQ[1] = true;
}