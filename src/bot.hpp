#ifndef BOT_HPP
#define BOT_HPP

#include <vector>
#include <limits>
#include <algorithm>

#include "board.hpp"
#include "move_generator.hpp"
#include "precomputed_move_data.hpp"

/// @brief Klasa szachowego SI
class Bot
{
    private:
        static struct
        {
            bool operator()(const Move& move1, const Move& move2) const
                { return assessMove(boardPtr, move1) > assessMove(boardPtr, move2); }
        } compare;  // Komparator uzywany przy wstepnej ocenie ruchow

        static Board* boardPtr; // Wskaznik do szachownicy uzywany w komparatorze

    public:
        Bot() = delete;

        /// @brief Wykonuje wybrany ruch na szachownicy
        /// @param board wskaznik do szachownicy, na ktorej zostanie wykonany ruch
        static void makeMove(Board* board);

    private:
        /// @brief Ocenia pozycje na szachownicy, wykorzystujac algorytm min-max i ciecia alfa-beta.
        /// @param board wskaznik do szachownicy, na ktorej zostanie wykonane przeszukiwanie
        /// @param depth glebokosc przeszukiwania
        /// @param alpha parametr alfa do ciec alfa-beta
        /// @param beta parametr beta do ciec alfa-beta
        /// @return Zwraca ocene pozycji na podanej szachownicy.
        static int search(const Board* board, const int& depth, int alpha, int beta);

        /// @brief Ocenia pozycje na szachownicy, biorac pod uwage same bicia. Wykorzystuje algorytm min-max z cieciami alfa-beta, bez ograniczenia glebokosci.
        /// @param board wskaznik do szachownicy, na ktorej zostanie wykonane przeszukiwanie
        /// @param alpha parametr alfa do ciec alfa-beta
        /// @param beta parametr beta do ciec alfa-beta
        /// @return Zwraca ocene pozycji na podanej szachownicy.
        static int searchAllCaptures(const Board* board, int alpha, int beta);
        
        /// @brief Wstepnie uklada ruchy w celu zoptymalizowania dzialania modyfikacji ciec alfa-beta.
        /// @param board wskaznik do szachownicy, na ktorej maja potencjalnie zostac wykonane ruchy
        /// @param moves std::vector potencjalnych posuniec 
        static inline void orderMoves(const Board* board, std::vector<Move>& moves);

        /// @brief Ocenia pozycje na szachownicy biorac pod uwage wazona ilosc materialu obu stron i potencjalny remis.
        /// @param board wskaznik do szachownicy do oceny
        /// @return Zwraca ocene pozycji, gdzie 0 to remis, <0 to przewaga przeciwnika, >0 to przewaga gracza.
        static int evaluate(const Board* board);

        /// @brief Wstepnie ocenia ruch na podstawie kilku trywialnych warunkow
        /// @param board wskaznik do szachownicy na ktorej ma potencjalnie zostac wykonany ruch
        /// @param move potencjalny ruch do wykonania
        /// @return Zwraca wstepna ocene ruchu.
        static int assessMove(const Board* board, const Move& move);

        /// @brief Liczy wazona srednia na podstawie wartosci bierek i ich umiejscowienia.
        /// @param board wskaznik do szachownicy, na ktorej zostana wykonane obliczenia
        /// @param pieceColor kolor gracza
        /// @return Zwraca liczbe materialu, gdzie 0 to remis, <0 to przewaga przeciwnika, >0 to przewaga gracza.
        static inline int countMaterial(const Board* board, const int& pieceColor);

        static const int values[7]; // Wartosci poszegolnych typow bierek

};

#endif