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

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <QList>
#include <QSet>

class Board;
class Square;
class GameState;

class Validator
{
public:
    Validator(const Board& board): m_board (board) {}
    QList<Square*> getLegalTargets(Square* from, const GameState& state) const;
    QSet<Square*> getNotInCheck(Square* from, const QList<Square*>& targets, Square *king) const;
private:
    bool isInCheck(Square* from, Square* to, Square* king) const noexcept;
    bool isCastlePathInCheck(Square* king, int direction) const noexcept;
    bool isPathClear(const Square* from, const Square* to) const noexcept;

    const Board& m_board;
};

#endif // VALIDATOR_H
