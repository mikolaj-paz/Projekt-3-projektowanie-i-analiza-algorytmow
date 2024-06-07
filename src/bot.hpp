#ifndef BOT_HPP
#define BOT_HPP

#include <vector>
#include <limits>
#include <algorithm>

#include "board.hpp"
#include "move_generator.hpp"
#include "precomputed_move_data.hpp"

#define NOW std::chrono::high_resolution_clock::now()
#define DURATION std::chrono::duration<double, std::milli>

class Bot
{
    private:
        static struct
        {
            bool operator()(const Move& move1, const Move& move2) const
                { return assessMove(boardPtr, move1) > assessMove(boardPtr, move2); }
        } compare;

        static Board* boardPtr;

    public:
        Bot() = delete;

        static bool makeMove(Board* board);

        static double time;
        static int iterations;

    private:
        // static int search(const Board* board, const int& depth, int alpha, int beta);
        static int search(const Board* board, const int& depth, int alpha, int beta);

        static int searchAllCaptures(const Board* board, int alpha, int beta);
        
        static inline void orderMoves(const Board* board, std::vector<Move>& moves);

        static int evaluate(const Board* board);

        static int assessMove(const Board* board, const Move& move);

        static inline int countMaterial(const Board* board, const int& pieceColor);

        static const int values[7];

};

#endif