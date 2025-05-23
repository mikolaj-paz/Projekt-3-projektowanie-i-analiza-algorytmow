#include "board.hpp"

Board::Board(const std::string FENstring): lastMove{Move(0,0,0,0)}, lastlastMove{Move(0,0,0,0)}, whitePiecesBitboard{0ULL}, blackPiecesBitboard{0ULL}, repetitionClock{0}
{
    std::fill(squares, squares + 64, 0);
    loadFEN(FENstring);
}

Board& Board::operator=(const Board& other)
{
    if (this == &other)
        return *this;

    blackToMove = other.blackToMove;

    whiteKingSquare = other.whiteKingSquare;
    blackKingSquare = other.blackKingSquare;

    whitePiecesBitboard = other.whitePiecesBitboard;
    blackPiecesBitboard = other.blackPiecesBitboard;

    lastMove = other.lastMove;
    lastlastMove = other.lastlastMove;

    repetitionClock = other.repetitionClock;
    halfMoveClock = other.halfMoveClock;
    
    for (int i = 0; i < 64; i++)
        squares[i] = other.squares[i];

    castleK[0] = other.castleK[0];
    castleQ[0] = other.castleQ[0];
    castleK[1] = other.castleK[1];
    castleQ[1] = other.castleQ[1];

    return *this;
}

void Board::update(const Move move)
{
    updateBitboards(move);

    if (squares[move.getTarget()] != 0 || move.getPiece() == Piece.pawn)
        halfMoveClock = 0;
    else
        ++halfMoveClock;

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
        castleQ[0] = false;
    if (castleK[0] && (move.getOrigin() == 7 || move.getTarget() == 7 || move.getOrigin() == 4))
        castleK[0] = false;
    if (castleQ[1] && (move.getOrigin() == 56 || move.getTarget() == 56 || move.getOrigin() == 60))
        castleQ[1] = false;
    if (castleK[1] && (move.getOrigin() == 63 || move.getTarget() == 63 || move.getOrigin() == 60))
        castleK[1] = false;

    if (lastlastMove == -move)
        ++repetitionClock;

    lastlastMove = lastMove;
    lastMove = move;
    blackToMove = !blackToMove;
}

void Board::updateBitboards(const Move& move)
{
    int pieceToMove = squares[move.getOrigin()];
    int pieceToCapture = squares[move.getTarget()];

    if ((pieceToMove & Piece.colorMask) == Piece.white)
    {
        if ((pieceToMove & Piece.typeMask) == Piece.king) whiteKingSquare = 1ULL << move.getTarget();
        whitePiecesBitboard ^= 1ULL << move.getOrigin() | 1ULL << move.getTarget();

        if (pieceToCapture != Piece.none)
            blackPiecesBitboard ^= 1ULL << move.getTarget();
        else if (move.castle()) // roszada
        {
            if (move.getTarget() == 6) // krolewska
                whitePiecesBitboard ^= 1ULL << 5 | 1ULL << 7;
            else // hetmanska
                whitePiecesBitboard ^= 1ULL << 0 | 1ULL << 3;
        }
        else if (move.enPassant())
            blackPiecesBitboard ^= 1ULL << (move.getTarget() - 8);
    }
    else
    {
        if ((pieceToMove & Piece.typeMask) == Piece.king) blackKingSquare = 1ULL << move.getTarget();
        blackPiecesBitboard ^= 1ULL << move.getOrigin() | 1ULL << move.getTarget();

        if (pieceToCapture != Piece.none)
            whitePiecesBitboard ^= 1ULL << move.getTarget();
        else if (move.castle()) // roszada
        {
            if (move.getTarget() == 62) // krolewska
                blackPiecesBitboard ^= 1ULL << 61 | 1ULL << 63;
            else // hetmanska
                blackPiecesBitboard ^= 1ULL << 56 | 1ULL << 59;
        }
        else if (move.enPassant())
            whitePiecesBitboard ^= 1ULL << (move.getTarget() + 8);
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
    while (i < FENstring.size() && c != ' ')
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
                (color == Piece.white ? whitePiecesBitboard : blackPiecesBitboard) |= 1ULL << rank * 8 + file;
                file++;
            }
        }

        c = FENstring[++i];
    }

    // ----------------------------------------------------
    // Informacje dodatkowe (nieobslugiwane)
    // ----------------------------------------------------

    while ((c = FENstring[++i]) == ' ') {}
    
    // Wybor tury
    if (c == 'w')
        blackToMove = false;
    else
        blackToMove = true;

    while ((c = FENstring[++i]) == ' ') {}

    // Roszady
    castleK[0] = false;
    castleQ[0] = false;
    castleK[1] = false;
    castleQ[1] = false;
    do
        switch (c)
        {
            case 'K':
                castleK[0] = true;
                break;
            case 'Q':
                castleQ[0] = true;
                break;
            case 'k':
                castleK[1] = true;
                break;
            case 'q':
                castleQ[1] = true;
                break;
        }
    while (((c = FENstring[++i]) != ' '));

    while ((c = FENstring[++i]) == ' ') {}

    if (c != '-')
    {
        int file = c - 'a';
        c = FENstring[++i];
        int rank = c - '0';

        int targetIndex = 8 * rank + file;
        int originIndex = targetIndex - 16;
        if (rank == 4)
            originIndex += 32;

        lastMove = Move(originIndex, targetIndex, Piece.pawn, Piece.none);
    }

    while ((c = FENstring[++i]) == ' ') {}

    std::string halfMoveString = std::to_string(c);
    while ((c = FENstring[++i]) != ' ')
        halfMoveString += c;
    halfMoveClock = std::stoi(halfMoveString);
}