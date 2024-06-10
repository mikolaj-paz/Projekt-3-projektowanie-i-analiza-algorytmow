#ifndef MOVE_GENERATOR_HPP
#define MOVE_GENERATOR_HPP

#include <vector>
#include <algorithm>
#include <math.h>
#include <random>
#include <chrono>

#include "precomputed_move_data.hpp"
#include "pieces.hpp"
#include "board.hpp"

/// @brief Klasa generujaca ruchy.
class MoveGenerator
{
    public:
        MoveGenerator() = delete;

        /// @brief Metoda uzywana do okreslenia stanu gry na szachownicy.
        /// @param board wskaznik do szachownicy do analizy
        /// @return Zwraca stan gry na szachownicy.
        static const GameState getBoardState(const Board* board);

        /// @param board wskaznik do szachownicy do analizy
        /// @param onlyCaptures warunek czy generowac tylko bicia
        /// @return Zwraca std::vector zawierajacy wszystkie legalne ruchy w pozycji.
        static std::vector<Move> getAllLegalMoves(const Board* board, bool onlyCaptures = false);

        /// @param board wskaznik do szachownicy do analizy
        /// @param square indeks pola, na ktorym znajduje sie bierka do analizy
        /// @param bitboard wskaznik do bitboard'a ruchow legalnych bierki
        /// @param onlyCaptures warunek czy generowac tylko bicia
        /// @return Zwraca std::vector zawierajacy wszystkie legalne ruchy bierki w pozycji.
        static std::vector<Move> getLegalMoves(const Board* board, int square, unsigned long long* bitboard, bool onlyCaptures = false);
        
        /// @param board wskaznik do szachownicy do analizy
        /// @param square indeks pola, na ktorym znajduje sie bierka do analizy
        /// @param bitboard wskaznik do bitboard'a ruchow pseudo-legalnych bierki
        /// @param friendlyFire warunek czy brac pod uwage atakowanie bierek wlasnego koloru
        /// @param onlyCaptures warunek czy generowac tylko bicia
        /// @return Zwraca std::vector zawierajacy wszystkie pseudo-legalne ruchy bierki w pozycji.
        static std::vector<Move> getMoves(const Board* board, int square, unsigned long long* bitboard, bool friendlyFire = false, bool onlyCaptures = false);

        /// @param board wskaznik do szachownicy do analizy
        /// @param friendlyPieceColor kolor gracza
        /// @return Zwraca bitboard zawierajacy pozycje pol atakowanych przez gracza.
        static unsigned long long attackedSquares(const Board* board, const int& friendlyPieceColor);

        /// @brief Sprawdza czy bialy krol jest szachowany w pozycji.
        /// @param board wskaznik do szachownicy do analizy
        /// @return Zwraca prawde jezeli krol jest atakowany, w przeciwnym wypadku falsz.
        static bool isWhiteKingInCheck(const Board* board)
        {
            if (attackedSquares(board, Piece.black) & board->getWhiteKingSquare())
                return true;
            return false;
        }

        /// @brief Sprawdza czy czarny krol jest szachowany w pozycji.
        /// @param board wskaznik do szachownicy do analizy
        /// @return Zwraca prawde jezeli krol jest atakowany, w przeciwnym wypadku falsz.
        static bool isBlackKingInCheck(const Board* board)
        {
            if (attackedSquares(board, Piece.white) & board->getBlackKingSquare())
                return true;
            return false;
        }

        /// @brief Sprawdza czy nastapila "martwa" pozycja.
        /// @param board wskaznik do szachownicy do analizy
        /// @return Zwraca prawde jezeli pozycja jest "martwa", w przeciwnym wypadku falsz.
        static bool isInsufficientMaterial(const Board* board);

    private:

        static const int directionOffsets[8];   // Marginesy kierunkow

        enum    // Indeksy kierunkow w kompatybilnych tablicach
        {
            TOP,
            BOT,
            LEFT,
            RIGHT,
            TOP_LEFT,
            BOT_RIGHT,
            TOP_RIGHT,
            BOT_LEFT
        };

        static const int knightOffsets[8];  // Marginesy skoczkow
        static const int pawnOffsets[6];    // Marginesy pionkow

        static const unsigned long long pawnStart[2];       // Maska pol startowych pionkow
        static const unsigned long long pawnPromotion[2];   // Maska pol o jeden przed promocja

        /// @brief Generuje ruchy pionowe i poziome.
        /// @param board wskaznik do tablicy ustawienia bierek na szachownicy
        /// @param square indeks zrodla generacji
        /// @return Zwraca wygenerowane ruchy jako maske.
        static unsigned long long generateOrthagonal(const int* board, const int& square);

        /// @brief Generuje ruchy ukosne.
        /// @param board wskaznik do tablicy ustawienia bierek na szachownicy
        /// @param square indeks zrodla generacji
        /// @return Zwraca wygenerowane ruchy jako maske. 
        static unsigned long long generateDiagonal(const int* board, const int& square);

        /// @brief Generuje ruchy krola
        /// @param board wskaznik do tablicy na ktorej zostana wygenerowane ruchy
        /// @param square indeks zrodla generacji
        /// @param friendlyPieceColor kolor gracza
        /// @return Zwraca wygenerowane ruchy jako maske.
        static unsigned long long generateKing(const Board* board, const int& square, const int& friendlyPieceColor);

        /// @brief Generuje ruchy pionka (bez atakow).
        /// @param board wskaznik do tablicy ustawienia bierek na szachownicy
        /// @param square indeks zrodla generacji
        /// @param friendlyPieceColor kolor gracza
        /// @return Zwraca wygenerowane ruchy jako maske.
        static unsigned long long generatePawn(const int* board, const int& square, const int& friendlyPieceColor);

        /// @brief Generuje ataki pionka.
        /// @param board wskaznik do tablicy na ktorej zostana wygenerowane ruchy
        /// @param square indeks zrodla generacji
        /// @param friendlyPieceColor kolor gracza
        /// @return Zwraca wygenerowane ruchy jako maske.
        static unsigned long long generatePawnAttacks(const Board* board, const int& square, const int& friendlyPieceColor);
};

#endif