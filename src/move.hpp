#ifndef MOVE_HPP
#define MOVE_HPP

#include <math.h>

#include "pieces.hpp"

/// @brief Klasa przechowujaca informacje o ruchu.
class Move
{
    public:
        /// @brief Domyslny kontruktor.
        /// @param originIndex indeks zrodlowy
        /// @param targetIndex indeks docelowy
        /// @param piece bierka poruszajaca sie
        /// @param capturePiece bierka zbijana (lub numer braku bierki)
        Move(const int& originIndex, const int& targetIndex, const int& piece, const int capturePiece):
            originSquare{originIndex}, targetSquare{targetIndex}, piece{piece}, capturePiece{capturePiece}
        {
            if ((piece & Piece.typeMask) == Piece.king && abs(targetIndex - originIndex) == 2)
                isCastle = true;
            else if ((piece & Piece.typeMask) == Piece.pawn && (abs(targetIndex - originIndex) % 8) != 0
                     && capturePiece == Piece.none)
                isEnPassant = true;
        }

        /// @brief Przeciazenie operatora odwrocenia. Zamienia indeks zrodlowy z docelowym.
        /// @return Zwraca nowy, odwrocony ruch.
        const Move operator-() const
            { return Move(targetSquare, originSquare, piece, capturePiece); }

        /// @brief Przeciazenie operatora porownania.
        /// @param l pierwszy ruch
        /// @param r drugi ruch
        /// @return Zwraca czy ruchy sa takie same.
        friend bool operator==(const Move& l, const Move& r)
        {
            return (
                l.originSquare == r.originSquare &&
                l.targetSquare == r.targetSquare &&
                l.piece == r.piece &&
                l.capturePiece == r.capturePiece
            );
        }

        /// @return Zwraca indeks zrodla ruchu.
        const int& getOrigin() const
            { return originSquare; }

        /// @return Zwraca indeks docelowy ruchu.
        const int& getTarget() const
            { return targetSquare; }

        /// @return Zwraca wartosc bierki poruszanej.
        const int& getPiece() const
            { return piece; }
            
        /// @return Zwraca wartosc bierki zbijanej.
        const int& getCapturePiece() const
            { return capturePiece; }

        /// @return Zwraca czy ruch jest biciem w przelocie.
        const bool& enPassant() const
            { return isEnPassant; }
        
        /// @return Zwraca czy ruch jest roszada.
        const bool& castle() const
            { return isCastle; }

    private:
        int originSquare;           // Indeks zrodla ruchu
        int targetSquare;           // Indeks celu ruchu

        bool isEnPassant = false;   // Wartosc okreslajaca czy ruch jest biciem w przelocie
        bool isCastle = false;      // Wartosc okreslajaca czy ruch jest roszada

        int piece;                  // Wartosc bierki poruszanej
        int capturePiece;           // Wartosc bierki zbijanej
};

#endif