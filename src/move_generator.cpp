#include "move_generator.hpp"

const int MoveGenerator::directionOffsets[8] = { 8, -8, -1, 1, 7, -7, 9, -9 };
const int MoveGenerator::knightOffsets[8] = { 15, 17, -17, -15, 6, -10, 10, -6 };
const int MoveGenerator::pawnOffsets[6] = { 7, 8, 9, -9, -8, -7 };

const unsigned long long MoveGenerator::pawnStart[2] = { 0xFF00, 0xFF000000000000 };
const unsigned long long MoveGenerator::pawnPromotion[2] = { 0xFF00000000000000, 0xFF };

std::vector<Move> MoveGenerator::getLegalMoves(const Board* board, int square, unsigned long long* bitboard)
{
    std::vector<Move> moves;

    int piece = board->get()[square]; 
    std::vector<Move> candidates = getMoves(board, square, bitboard);

    for (auto & candidate : candidates)
    {
        Board temp = *board;
        temp.update(candidate);
        if (!((piece & Piece.colorMask) == Piece.white ? isWhiteKingInCheck(&temp) : isBlackKingInCheck(&temp)))
            moves.push_back(candidate);
        else
            *bitboard ^= 1ULL << candidate.getTarget();
    }

    return moves;
}

std::vector<Move> MoveGenerator::getMoves(const Board* board, int square, unsigned long long* bitboard, bool friendlyFire)
{
    std::vector<Move> moves;

    int piece = board->get()[square];
    int pieceColor = piece & Piece.colorMask;

    switch (piece & Piece.typeMask)
    {
        case 1:
            *bitboard = generatePawn(board->get(), square, pieceColor) | generatePawnAttacks(board, square, pieceColor);
            break;
        case 2:
            *bitboard = PrecomputedMoveData::knightAttacks(square);
            break;
        case 3:
            *bitboard = generateDiagonal(board->get(), square, pieceColor);
            break;
        case 4:
            *bitboard = generateOrthagonal(board->get(), square, pieceColor);
            break;
        case 5:
            *bitboard = generateOrthagonal(board->get(), square, pieceColor) | generateDiagonal(board->get(), square, pieceColor);
            break;
        case 6:
            *bitboard = generateKing(board, square, pieceColor);
            break;
        default:
            return moves;
    }

    if (!friendlyFire)
        *bitboard ^= (board->toMove() == Piece.white ? board->getWhiteBitboard() & *bitboard : board->getBlackBitboard() & *bitboard); 

    for (int i = 0; i < 64; i++)
    {
        if (((1ULL << i) & *bitboard) >> i)
            moves.push_back(Move(square, i, piece));
    }

    return moves;
}

unsigned long long MoveGenerator::attackedSquares(const Board* board, const int& pieceColor)
{
    unsigned long long attacked = 0ULL;
    const int* squares = board->get();

    for (int i = 0; i < 64; i++)
        if (squares[i] != 0 && (squares[i] & Piece.colorMask) == pieceColor)
        {
            unsigned long long singlePieceBitboard = 0ULL;
            if (squares[i] == (Piece.white | Piece.pawn))
                singlePieceBitboard = PrecomputedMoveData::whitePawnAttacks(i);
            else if (squares[i] == (Piece.black | Piece.pawn))
                singlePieceBitboard = PrecomputedMoveData::blackPawnAttacks(i);
            else if ((squares[i] & Piece.typeMask) == Piece.king)
                singlePieceBitboard = PrecomputedMoveData::kingAttacks(i);
            else
                getMoves(board, i, &singlePieceBitboard, true);
            attacked |= singlePieceBitboard;
        }
    
    return attacked;
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
            legalMoves |= 1ULL << squareIndex;
            if (board[squareIndex] != 0) break;  
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
            legalMoves |= 1ULL << squareIndex;
            if (board[squareIndex] != 0) break;
        }
    }

    return legalMoves;
}

unsigned long long MoveGenerator::generateKing(const Board* board, const int& square, const int& friendlyPieceColor)
{
    unsigned long long legalMoves = PrecomputedMoveData::kingAttacks(square);
    const int* squares = board->get();

    // Roszady
    int squareIndex;
    if (friendlyPieceColor == Piece.white)
    {
        unsigned long long blackAttacked = attackedSquares(board, Piece.black);

        bool canCastleK = board->getCastleK()[0];
        bool canCastleQ = board->getCastleQ()[0];

        // Krolewska
        squareIndex = square + 2;
        if (canCastleK && !(squares[squareIndex] + squares[squareIndex - 1]) && !(blackAttacked & (1ULL << squareIndex ^ 1ULL << squareIndex - 1)))
            legalMoves |= 1ULL << squareIndex;

        // Hetmanska
        squareIndex = square - 2;
        if (canCastleQ && !(squares[squareIndex] + squares[squareIndex + 1]) && !(blackAttacked & (1ULL << squareIndex ^ 1ULL << squareIndex + 1)))
            legalMoves |= 1ULL << squareIndex;
    }
    else
    {
        unsigned long long whiteAttacked = attackedSquares(board, Piece.white);

        bool canCastleK = board->getCastleK()[1];
        bool canCastleQ = board->getCastleQ()[1];

        // Krolewska
        squareIndex = square + 2;
        if (canCastleK && !(squares[squareIndex] + squares[squareIndex - 1]) && !(whiteAttacked & (1ULL << squareIndex ^ 1ULL << squareIndex - 1)))
            legalMoves |= 1ULL << squareIndex;

        // Hetmanska
        squareIndex = square - 2;
        if (canCastleQ && !(squares[squareIndex] + squares[squareIndex + 1]) && !(whiteAttacked & (1ULL << squareIndex ^ 1ULL << squareIndex + 1)))
            legalMoves |= 1ULL << squareIndex;
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