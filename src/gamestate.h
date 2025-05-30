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

#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <QList>
#include "pieces.h"

class Square;

struct PlayerState {
    Square* m_king;
    bool m_kingSideCastleRight = true;
    bool m_queenSideCastleRight = true;
    // last move
    Square* m_enPassantTarget = nullptr;
    Square* m_promotedPawnSquare = nullptr;
    std::optional<QChar> m_captured;
    // last moves
    int m_noPawnMoveOrCapture = 0;
    QList<std::pair<Square*, Square*>> m_moves;
};


struct GameState
{
    PlayerState m_white;
    PlayerState m_black;
    Pieces::Color m_turnColor = Pieces::White;
    void switchTurn() noexcept {
        m_turnColor = static_cast<Pieces::Color>(
            1 - static_cast<int>(m_turnColor));
    }
    PlayerState& playerState() noexcept {
        return (m_turnColor == Pieces::White) ? m_white: m_black;
    }
    PlayerState& opponentState() noexcept {
        return (m_turnColor == Pieces::White) ? m_black: m_white;
    }

    const PlayerState& playerState() const noexcept {
        return (m_turnColor == Pieces::White) ? m_white: m_black;
    }
    const PlayerState& opponentState() const noexcept {
        return (m_turnColor == Pieces::White) ? m_black: m_white;
    }
};

#endif // GAMESTATE_H
