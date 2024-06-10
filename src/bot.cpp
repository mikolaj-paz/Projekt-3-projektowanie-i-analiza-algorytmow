#include "bot.hpp"

Board* Bot::boardPtr = nullptr;

const int Bot::values[7] = {
    0,   // none
    100, // pawn
    300, // knight
    300, // bishop
    500, // rook
    800, // queen
    0    // king
};

void Bot::makeMove(Board* board)
{
    std::vector<Move> candidates = MoveGenerator::getAllLegalMoves(board);
    if (!candidates.size()) 
        return;

    orderMoves(board, candidates);

    Move* bestMove;
    int bestMoveEval = INT_MIN;

    for (auto & candidate : candidates)
    {
        Board temp = *board;
        temp.update(candidate);
        int candidateEval = -search(&temp, 2, INT_MIN + 1, INT_MAX - 1);
        if (candidateEval > bestMoveEval)
        {
            bestMove = &candidate;
            bestMoveEval = candidateEval;
        }
    }

    board->update(*bestMove);
}

int Bot::search(const Board* board, const int& depth, int alpha, int beta)
{
    if (!depth)
        return searchAllCaptures(board, alpha, beta);

    std::vector moves = MoveGenerator::getAllLegalMoves(board);
    if (!moves.size())
    {
        int toMove = board->toMove();
        if ((toMove == Piece.white && MoveGenerator::isWhiteKingInCheck(board))
            || (toMove == Piece.black && MoveGenerator::isBlackKingInCheck(board)))
            return INT_MIN + 1;
        return 0;
    }
    orderMoves(board, moves);

    for (auto & move : moves)
    {
        Board temp = *board;
        temp.update(move);
        int eval = -search(&temp, depth - 1, -beta, -alpha);

        if (eval >= beta)
            return beta;
        
        if (eval > alpha)
            alpha = eval;
    }

    return alpha;
}

int Bot::searchAllCaptures(const Board* board, int alpha, int beta)
{
    int eval = evaluate(board);

    if (eval >= beta)
        return beta;
    
    if (eval > alpha)
        alpha = eval;

    std::vector moves = MoveGenerator::getAllLegalMoves(board, true);

    for (auto & move : moves)
    {
        Board temp = *board;
        temp.update(move);
        eval = -searchAllCaptures(&temp, -beta, -alpha);

        if (eval >= beta)
            return beta;

        if (eval > alpha)
            alpha = eval;
    }

    return alpha;
}

inline void Bot::orderMoves(const Board* board, std::vector<Move>& moves)
{
    Board* temp = boardPtr;
    boardPtr = const_cast<Board*>(board);
    std::sort(moves.begin(), moves.end(), compare);
    boardPtr = temp;
}

int Bot::evaluate(const Board* board)
{
    if (MoveGenerator::isInsufficientMaterial(board))
        return 0;

    int whiteEval = countMaterial(board, Piece.white);
    int blackEval = countMaterial(board, Piece.black);

    int evaluation = whiteEval - blackEval;

    int perspective = board->toMove() == Piece.white ? 1 : -1;

    return evaluation * perspective;
}

int Bot::assessMove(const Board* board, const Move& move)
{
    int moveScore = 0;
    int toMove = move.getPiece() & Piece.colorMask;

    int movePieceType = move.getPiece() & Piece.typeMask;
    int capturePieceType = move.getCapturePiece() & Piece.typeMask;

    if (capturePieceType != Piece.none)
        moveScore = 10 * values[capturePieceType] - values[movePieceType];
    
    if (movePieceType == Piece.pawn && (move.getTarget() % 8 == 7 || move.getTarget() % 8 == 7))
        moveScore += values[Piece.queen];
    
    if ((toMove == Piece.white ? MoveGenerator::attackedSquares(board, Piece.black) : MoveGenerator::attackedSquares(board, Piece.white))
        & 1ULL << move.getTarget())
        moveScore -= values[movePieceType];
    
    return moveScore;
}

inline int Bot::countMaterial(const Board* board, const int& pieceColor)
{
    int material = 0;

    const int* squares = board->get();
    for (int i = 0; i < 64; i++)
        if ((squares[i] & Piece.colorMask) == pieceColor)
            material += values[squares[i] & Piece.typeMask] + PrecomputedMoveData::squareScore(i, squares[i]);

    return material;
}
    