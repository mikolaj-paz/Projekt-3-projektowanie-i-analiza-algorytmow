#ifndef PRECOMPUTED_MOVE_DATA_HPP
#define PRECOMPUTED_MOVE_DATA_HPP

#include "pieces.hpp"

/// @brief Klasa obliczajaca niektore dane, ktore nie wymagaja liczenia w czasie rzeczywistym.
class PrecomputedMoveData
{
    public:
        PrecomputedMoveData() = delete;

        /// @brief Metoda inicjujaca wszystkie obliczenia wstepne.
        static void init();

        /// @param square indeks pola pionka
        /// @return Zwraca maske atakow bialego pionka na danym polu.
        static const unsigned long long& whitePawnAttacks(const int& square)
            { return whitePawnAttackBitboards[square]; }

        /// @param square indeks pola pionka
        /// @return Zwraca maske atakow czarnego pionka na danym polu.
        static const unsigned long long& blackPawnAttacks(const int& square)
            { return blackPawnAttackBitboards[square]; }

        /// @param square indeks pola krola
        /// @return Zwraca maske atakow krola na danym polu.
        static const unsigned long long& kingAttacks(const int& square)
            { return kingAttackBitboards[square]; }

        /// @param square indeks pola skoczka
        /// @return Zwraca maske atakow skoczka na danym polu.
        static const unsigned long long& knightAttacks(const int& square)
            { return knightAttackBitboards[square]; }

        /// @param square indeks pola na szachownicy
        /// @param piece wartosc bierki
        /// @return Zwraca ocene pola na ktorym znajduje sie dana figura.
        static const int& squareScore(const int& square, const int& piece)
            { return pieceSquareScore[piece][square]; }

    private:
        static unsigned long long whitePawnAttackBitboards[64];
        static unsigned long long blackPawnAttackBitboards[64];

        static unsigned long long kingAttackBitboards[64];

        static unsigned long long knightAttackBitboards[64];

        static int pieceSquareScore[15][64];
};

#endif