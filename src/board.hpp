#ifndef BOARD_HPP
#define BOARD_HPP

#include <string>
#include <map>

#include "pieces.hpp"

class Board
{
    public:
        Board();

        ~Board()
            { delete[] squares; }

        const int* const get() const
            { return squares; }

        void update(const int& square, const int& value)
            { squares[square] = value; }

        void loadFEN(const std::string FENstring = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    private:
        int* squares;
};

#endif