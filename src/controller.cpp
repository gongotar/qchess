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

#include "controller.h"
#include "square.h"
#include "board.h"

namespace {
QList<QChar> promotionChoices (Pieces::Color color)
{
    if (color == Pieces::Black)
        return {Pieces::BlackQueen,
                Pieces::BlackRook,
                Pieces::BlackKnight,
                Pieces::BlackBishop};
    else
        return {Pieces::WhiteQueen,
                Pieces::WhiteRook,
                Pieces::WhiteKnight,
                Pieces::WhiteBishop};
}
}

Controller::Controller(Board *board, QObject *parent) :
    QObject(parent),
    m_board (board),
    m_validator(board),
    m_moveExec(board)
{
    m_states[Pieces::White].m_king = m_board->at(7, 4);
    m_states[Pieces::Black].m_king = m_board->at(0, 4);
}

void Controller::selectOrMovePiece(int row, int col)
{    
    if (m_prevMove) {
        m_prevMove->first->setHighlight(false);
        m_prevMove->second->setHighlight(false);
        m_prevMove.reset();
    }
    for (Square* sq : std::as_const(m_targets))
        sq->setLegalDestination(false);
    if (m_from)
        m_from->setIsSelected(false);

    Square* selected = m_board->at(row, col);
    GameState& state = m_states[m_turnColor];

    if (!m_from || !m_targets.contains(selected)) {
        if (Pieces::pieceColor(selected->piece()) != m_turnColor)
            return;
        QList <Square*> targets = m_validator.getLegalTargets(selected, state);
        m_targets = m_validator.getNotInCheck(selected, targets, state.m_king);
        m_from = selected;

        for (Square* sq : std::as_const(m_targets))
            sq->setLegalDestination(true);
        selected->setIsSelected(true);
        return;
    }
    else if (m_from) {
        MoveExecutor::MoveResult res = m_moveExec(m_from, selected);

        // update state
        state.m_kingSideCastleRight &= !res.m_revokeKingSideCastleRight;
        state.m_queenSideCastleRight &= !res.m_revokeQueenSideCastleRight;
        m_states[1-m_turnColor].m_enPassantTarget = res.m_enPassantTarget;
        state.m_enPassantTarget = nullptr;
        if (res.m_newKingSquare)
            state.m_king = res.m_newKingSquare;
        if (const Square* p = res.m_promotedPawnSquare; p)
            emit promotePawn(p->row(), p->col(), promotionChoices(m_turnColor));
        // update ui
        m_from->setHighlight(true);
        selected->setHighlight(true);
        m_prevMove.emplace(m_from, selected);

        m_turnColor = static_cast<Pieces::Color>(1 - static_cast<int>(m_turnColor));
    }
    m_from = nullptr;
    m_targets.clear();

}

void Controller::promotePawnTo(int row, int col, const QChar &piece) {
    m_board->at(row, col)->setPiece(piece);
}
