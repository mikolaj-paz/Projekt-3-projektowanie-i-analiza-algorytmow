#include "move_generator.hpp"

const int MoveGenerator::directionOffsets[8] = { 8, -8, -1, 1, 7, -7, 9, -9 };
const int MoveGenerator::knightOffsets[8] = { 15, 17, -17, -15, 6, -10, 10, -6 };
const int MoveGenerator::pawnOffsets[6] = { 7, 8, 9, -9, -8, -7 };

const unsigned long long MoveGenerator::pawnStart[2] = { 0xFF00, 0xFF000000000000 };
const unsigned long long MoveGenerator::pawnPromotion[2] = { 0xFF00000000000000, 0xFF };

bool* MoveGenerator::getLegalMoves(const Board* board, int square, int piece)
{
    bool* moves = new bool[64];
    std::fill(moves, moves + 64, 0);

    int pieceColor = piece & Piece.colorMask;

    if (!piece || pieceColor != board->toMove()) return moves;

    unsigned long long bitboard;

    switch (piece & Piece.typeMask)
    {
        case 1:
            bitboard = generatePawn(board->get(), square, pieceColor) | generatePawnAttacks(board, square, pieceColor);
            break;
        case 2:
            bitboard = generateKnight(board->get(), square, pieceColor);
            break;
        case 3:
            bitboard = generateDiagonal(board->get(), square, pieceColor);
            break;
        case 4:
            bitboard = generateOrthagonal(board->get(), square, pieceColor);
            break;
        case 5:
            bitboard = generateOrthagonal(board->get(), square, pieceColor) | generateDiagonal(board->get(), square, pieceColor);
            break;
        case 6:
            bitboard = generateKing(board->get(), square, pieceColor);
            break;
        default:
            return nullptr;
    }

    for (int i = 0; i < 64; i++)
    {
        moves[i] = ((1ULL << i) & bitboard) >> i;
        unsigned long long tmp = (1UL << i);
    }

    return moves;
}

unsigned long long MoveGenerator::generateOrthagonal(const int* board, const int& square, const int& friendlyPieceColor)
{
    unsigned long long legalMoves = 0ULL;

    int margin[4];

    margin[BOT] = square / 8;
    margin[TOP] = 7 - margin[BOT];

    margin[LEFT] = square % 8;
    margin[RIGHT] = 7 - margin[LEFT];

    int squareIndex;
    for (int j = 0; j < 4; j++)
    {
        for (int i = 1; i <= margin[j]; i++)
        {
            squareIndex = square + i * directionOffsets[j];
            if (board[squareIndex] == 0)
                legalMoves |= 1ULL << squareIndex;
            else
            {
                if ((board[squareIndex] & Piece.colorMask) != friendlyPieceColor)
                    legalMoves |= 1ULL << squareIndex;
                break;
            }
                
        }
    }

    return legalMoves;
}

unsigned long long MoveGenerator::generateDiagonal(const int* board, const int& square, const int& friendlyPieceColor)
{
    unsigned long long legalMoves = 0ULL;

    int margin[8];

    margin[BOT] = square / 8;
    margin[TOP] = 7 - margin[BOT];
    margin[LEFT] = square % 8;
    margin[RIGHT] = 7 - margin[LEFT];

    margin[TOP_LEFT] = margin[TOP] < margin[LEFT] ? margin[TOP] : margin[LEFT];
    margin[TOP_RIGHT] = margin[TOP] < margin[RIGHT] ? margin[TOP] : margin[RIGHT];
    margin[BOT_LEFT] = margin[BOT] < margin[LEFT] ? margin[BOT] : margin[LEFT];
    margin[BOT_RIGHT] = margin[BOT] < margin[RIGHT] ? margin[BOT] : margin[RIGHT];

    int squareIndex;
    for (int j = 4; j < 8; j++)
    {
        for (int i = 1; i <= margin[j]; i++)
        {
            squareIndex = square + i * directionOffsets[j];
            if (board[squareIndex] == 0)
                legalMoves |= 1ULL << squareIndex;
            else
            {
                if ((board[squareIndex] & Piece.colorMask) != friendlyPieceColor)
                    legalMoves |= 1ULL << squareIndex;
                break;
            }
        }
    }

    return legalMoves;
}

unsigned long long MoveGenerator::generateKing(const int* board, const int& square, const int& friendlyPieceColor)
{
    unsigned long long legalMoves = 0ULL;
    int margin[8];

    margin[BOT] = square / 8;
    margin[TOP] = 7 - margin[BOT];
    margin[LEFT] = square % 8;
    margin[RIGHT] = 7 - margin[LEFT];

    margin[TOP_LEFT] = margin[TOP] < margin[LEFT] ? margin[TOP] : margin[LEFT];
    margin[TOP_RIGHT] = margin[TOP] < margin[RIGHT] ? margin[TOP] : margin[RIGHT];
    margin[BOT_LEFT] = margin[BOT] < margin[LEFT] ? margin[BOT] : margin[LEFT];
    margin[BOT_RIGHT] = margin[BOT] < margin[RIGHT] ? margin[BOT] : margin[RIGHT];

    int squareIndex;
    for (int j = 0; j < 8; j++)
    {
        if (margin[j] > 0)
        {
            squareIndex = square + directionOffsets[j];
            if (board[squareIndex] == 0)
                legalMoves |= 1ULL << squareIndex;
            else if ((board[squareIndex] & Piece.colorMask) != friendlyPieceColor)
                legalMoves |= 1ULL << squareIndex;
        }
    }

    return legalMoves;
}

unsigned long long MoveGenerator::generateKnight(const int* board, const int& square, const int& friendlyPieceColor)
{
    unsigned long long legalMoves = 0ULL;

    int margin[4];

    margin[BOT] = square / 8;
    margin[TOP] = 7 - margin[BOT];
    margin[LEFT] = square % 8;
    margin[RIGHT] = 7 - margin[LEFT];

    int squareIndex;
    int j = 0;
    for (int k = 0; k < 2; k++)
    {   
        for (int l = 2; l < 4; l++)
        {
            if (margin[k] > 1 && margin[l] > 0)
            {
                squareIndex = square + knightOffsets[j];
                if (board[squareIndex] == 0 || (board[squareIndex] & Piece.colorMask) != friendlyPieceColor)
                    legalMoves |= 1ULL << squareIndex;
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
                if (board[squareIndex] == 0 || (board[squareIndex] & Piece.colorMask) != friendlyPieceColor)
                    legalMoves |= 1ULL << squareIndex;
            }
            j++;
        }
    }

    return legalMoves;
}

unsigned long long MoveGenerator::generatePawn(const int* board, const int& square, const int& friendlyPieceColor)
{
    unsigned long long legalMoves = 0ULL;
    
    int colorIndex = 0;
    int offset = 0;
    if (friendlyPieceColor == Piece.black)
    {
        offset = 3;
        colorIndex = 1;
    }

    int squareIndex = square + pawnOffsets[offset + 1];
    if (board[squareIndex] == 0)
    {
        legalMoves |= 1ULL << squareIndex;
        squareIndex += pawnOffsets[offset + 1];
        if ((1ULL << square & pawnStart[colorIndex]) >> square && board[squareIndex] == 0)
            legalMoves |= 1ULL << squareIndex;
    }

    return legalMoves;
}

unsigned long long MoveGenerator::generatePawnAttacks(const Board* board, const int& square, const int& friendlyPieceColor)
{
    unsigned long long legalMoves = 0ULL;
    const int* squares = board->get();
    
    int offset = 0;
    if (friendlyPieceColor == Piece.black)
        offset = 3;

    int squareIndex = square + pawnOffsets[offset];
    if (square % 8 != 0)
    { 
        if (squares[squareIndex] != 0 && (squares[squareIndex] & Piece.colorMask) != friendlyPieceColor)
            legalMoves |= 1ULL << squareIndex;
        // en passant
        else if ((squares[squareIndex] == 0
                  && squares[square - 1] & Piece.typeMask) == Piece.pawn
                  && board->getLastMove().getTarget() == square - 1
                  && abs(board->getLastMove().getTarget() - board->getLastMove().getOrigin()) == 16)
            legalMoves |= 1ULL << squareIndex;
    }

    squareIndex = square + pawnOffsets[offset + 2];
    if (square % 8 != 7)
    {
        if (squares[squareIndex] != 0 && (squares[squareIndex] & Piece.colorMask) != friendlyPieceColor)
            legalMoves |= 1ULL << squareIndex;
        // en passant
        else if ((squares[squareIndex] == 0
                  && squares[square + 1] & Piece.typeMask) == Piece.pawn
                  && board->getLastMove().getTarget() == square + 1
                  && abs(board->getLastMove().getTarget() - board->getLastMove().getOrigin()) == 16)
            legalMoves |= 1ULL << squareIndex;
    }

    return legalMoves;
}