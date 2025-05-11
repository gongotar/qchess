#ifndef PIECES_H
#define PIECES_H

#include <QChar>

namespace Pieces
{
    constexpr QChar Empty       = QChar(' ');

    constexpr char16_t WhiteKingCode   = 0x2654;
    constexpr char16_t WhiteQueenCode  = 0x2655;
    constexpr char16_t WhiteRookCode   = 0x2656;
    constexpr char16_t WhiteBishopCode = 0x2657;
    constexpr char16_t WhiteKnightCode = 0x2658;
    constexpr char16_t WhitePawnCode   = 0x2659;

    constexpr char16_t BlackKingCode   = 0x265A;
    constexpr char16_t BlackQueenCode  = 0x265B;
    constexpr char16_t BlackRookCode   = 0x265C;
    constexpr char16_t BlackBishopCode = 0x265D;
    constexpr char16_t BlackKnightCode = 0x265E;
    constexpr char16_t BlackPawnCode   = 0x265F;

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
        Black = 0,
        White,
        None,
    };

    inline Color pieceColor(const QChar &piece) noexcept
    {
        const ushort code = piece.unicode();
        if (code >= BlackKing && code <= BlackPawn)
            return Black;
        else if (code >= WhiteKing && code <= WhitePawn)
            return White;
        return None;
    }
}

#endif // PIECES_H
