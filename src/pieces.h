// MIT License
//
// Copyright (c) 2025 Masoud Jami
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
