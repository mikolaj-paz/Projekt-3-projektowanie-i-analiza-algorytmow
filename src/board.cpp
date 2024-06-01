#include "board.hpp"

Board::Board()
{
    squares = new int[64];
    for (int i = 0; i < 64; i++)
        squares[i] = 0;

    loadFEN();
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
                file++;
            }
        }

        c = FENstring[++i];
    }
}