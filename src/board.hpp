#ifndef BOARD_HPP
#define BOARD_HPP

#include <string>
#include <map>
#include <algorithm>

#include "pieces.hpp"
#include "move.hpp"

/// @brief Enumerator stanu rozgrywki
enum GameState
{
    GAME_ACTIVE,
    GAME_WIN,
    GAME_DRAW
};

/// @brief Klasa szachownicy
class Board
{
    public:
        /// @brief Domyslny kontruktor klasy szachownicy
        /// @param FENstring Notacja Forsytha-Edwardsa, z ktorej zostanie odczytana pozycja startowa
        Board(const std::string FENstring = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

        /// @brief Konstruktor kopiujacy
        /// @param other druga szachownica
        Board(const Board& other):
            blackToMove{other.blackToMove}, 
            whiteKingSquare{other.whiteKingSquare}, blackKingSquare{other.blackKingSquare},
            whitePiecesBitboard{other.whitePiecesBitboard}, blackPiecesBitboard{other.blackPiecesBitboard},
            lastMove{other.lastMove}, lastlastMove{other.lastlastMove},
            repetitionClock{other.repetitionClock}, halfMoveClock{other.halfMoveClock}
        {
            for (int i = 0; i < 64; i++)
                squares[i] = other.squares[i];

            castleK[0] = other.castleK[0];
            castleQ[0] = other.castleQ[0];
            castleK[1] = other.castleK[1];
            castleQ[1] = other.castleQ[1];
        }

        /// @brief Przeciazenia operatora kopiujacego-przypisywania
        /// @param other druga szachownica
        /// @return Zwraca referencje do zmienionej szachownicy.
        Board& operator=(const Board& other);

        /// @return Zwraca tablice z bierkami na szachownicy.
        const int* const get() const
            { return squares; }

        /// @return Zwraca ruch poprzedzajacy obecna pozycje.
        const Move& getLastMove() const
            { return lastMove; }

        /// @return Zwraca dwuelementowa tablice roszad po stronie krolewskiej.
        const bool* getCastleK() const
            { return castleK; }

        /// @return Zwraca dwuelementowa tablice roszad po stronie hetmanskiej.
        const bool* getCastleQ() const
            { return castleQ; }

        /// @return Zwraca maske pozycji bialego krola.
        const unsigned long long& getWhiteKingSquare() const
            { return whiteKingSquare; }

        /// @return Zwraca maske pozycji czarnego krola.
        const unsigned long long& getBlackKingSquare() const
            { return blackKingSquare; }

        /// @return Zwraca maske pozycji bialych bierek.
        const unsigned long long& getWhiteBitboard() const
            { return whitePiecesBitboard; }

        /// @return Zwraca maske pozycji czarnych bierek.
        const unsigned long long& getBlackBitboard() const
            { return blackPiecesBitboard; }

        /// @brief Wykonuje ruch na szachownicy
        /// @param move ruch do wykonania
        void update(const Move move);

        /// @return Zwraca kolor, ktory rusza sie jako nastepny.
        const int toMove() const
            { return blackToMove ? Piece.black : Piece.white; }

        /// @return Zwraca liczbe powtorzen poprzedzajacych obecna pozycje.
        const int& getRepetitionCount() const
            { return repetitionClock; }

        /// @return Zwraca liczbe ruchow polowicznych od ostatniego poruszenia pionem lub bicia.
        const int& getHalfmoveCount() const
            { return halfMoveClock; }

        /// @brief Aktualizuje wszystkie bitboard'y.
        /// @param move ruch do wykonania w pozycji
        void updateBitboards(const Move& move);

        /// @brief Aktualizuje szachownice wedlug Notacji Forsytha-Edwardsa.
        /// @param FENstring Notacja Forsytha-Edwardsa, z ktorej zostanie odczytana pozycja
        void loadFEN(const std::string FENstring);
        
    private:
        int squares[64];    // Tablica przechowujaca pozycje bierek na szachownicy
        bool blackToMove;   // Czy czarny nastepny do wykonania ruchu

        bool castleK[2];    // Roszady krolewskie
        bool castleQ[2];    // Roszady hetmanskie

        unsigned long long whitePiecesBitboard; // Bitboard bialych bierek
        unsigned long long blackPiecesBitboard; // Bitboard czarnych bierek

        unsigned long long whiteKingSquare;     // Maska bialego krola
        unsigned long long blackKingSquare;     // Maska czarnego krola

        Move lastMove;          // Ostatni wykonany ruch
        Move lastlastMove;      // Przedostatni wykonany ruch

        int repetitionClock;    // Liczba powtorzen w ruchach poprzedzajacych pozycje
        int halfMoveClock;      // Liczba wykonanych ruchow polowicznych od ostatniego poruszenia pionem lub bicia
};

#endif