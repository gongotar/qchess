#ifndef PIECES_H
#define PIECES_H

#include <QChar>

namespace Pieces
{
    constexpr QChar Empty       = QChar(' ');

    constexpr char16_t WhiteKingCode   = 'k';
    constexpr char16_t WhiteQueenCode  = 'q';
    constexpr char16_t WhiteRookCode   = 'r';
    constexpr char16_t WhiteBishopCode = 'b';
    constexpr char16_t WhiteKnightCode = 'h';
    constexpr char16_t WhitePawnCode   = 'p';

    constexpr char16_t BlackKingCode   = 'l';
    constexpr char16_t BlackQueenCode  = 'w';
    constexpr char16_t BlackRookCode   = 't';
    constexpr char16_t BlackBishopCode = 'n';
    constexpr char16_t BlackKnightCode = 'j';
    constexpr char16_t BlackPawnCode   = 'o';

    constexpr QChar WhiteKing   = QChar(WhiteKingCode);
    constexpr QChar WhiteQueen  = QChar(WhiteQueenCode);
    constexpr QChar WhiteRook   = QChar(WhiteRookCode);
    constexpr QChar WhiteBishop = QChar(WhiteBishopCode);
    constexpr QChar WhiteKnight = QChar(WhiteKnightCode);
    constexpr QChar WhitePawn   = QChar(WhitePawnCode);

    constexpr QChar BlackKing   = QChar(BlackKingCode);
    constexpr QChar BlackQueen  = QChar(BlackQueenCode);
    constexpr QChar BlackRook   = QChar(BlackRookCode);
    constexpr QChar BlackBishop = QChar(BlackBishopCode);
    constexpr QChar BlackKnight = QChar(BlackKnightCode);
    constexpr QChar BlackPawn   = QChar(BlackPawnCode);

    enum Color: int
    {
        White = 0,
        Black,
        None,
    };

    inline Color pieceColor(const QChar &piece) noexcept
    {
        switch(ushort code = piece.unicode()) {
            case WhiteKingCode:
            case WhiteQueenCode:
            case WhiteRookCode:
            case WhiteBishopCode:
            case WhiteKnightCode:
            case WhitePawnCode:
                return White;
            case BlackKingCode:
            case BlackQueenCode:
            case BlackRookCode:
            case BlackBishopCode:
            case BlackKnightCode:
            case BlackPawnCode:
                return Black;
            default:
                return None;
        }
    }
}

#endif // PIECES_H
