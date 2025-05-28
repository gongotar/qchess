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

Controller::Controller(QObject *parent) :
    QObject(parent),
    m_board(this),
    m_validator(m_board),
    m_moveExec(m_board)
{
    m_states[Pieces::White].m_king = m_board.at(7, 4);
    m_states[Pieces::Black].m_king = m_board.at(0, 4);
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

    Square* selected = m_board.at(row, col);
    GameState& state = m_states[m_turnColor];

    if (!m_from || !m_targets.contains(selected)) {
        if (Pieces::pieceColor(selected->piece()) != m_turnColor)
            return;
        m_targets = m_validator.getLegalTargets(selected, state);
        m_from = selected;

        for (Square* sq : std::as_const(m_targets))
            sq->setLegalDestination(true);
        selected->setIsSelected(true);
        return;
    }
    else if (m_from) {
        m_moveExec(m_from, selected, m_states);

        if (const Square* p = state.m_promotedPawnSquare; p)
            emit promotePawn(p->row(), p->col(), promotionChoices(m_turnColor));
        // update ui
        m_from->setHighlight(true);
        selected->setHighlight(true);
        m_prevMove.emplace(m_from, selected);

        handleGameOutCome(m_validator.evaluateGameOutcome(m_states[1-m_turnColor]));

        m_turnColor = static_cast<Pieces::Color>(1 - static_cast<int>(m_turnColor));
    }
    m_from = nullptr;
    m_targets.clear();
}

void Controller::promotePawnTo(int row, int col, const QChar &piece) {
    m_board.at(row, col)->setPiece(piece);
}

void Controller::restartGame()
{
    m_board.resetBoard();
    m_states[Pieces::White] = GameState{};
    m_states[Pieces::Black] = GameState{};
    m_states[Pieces::White].m_king = m_board.at(7, 4);
    m_states[Pieces::Black].m_king = m_board.at(0, 4);
    m_from = nullptr;
    m_prevMove.reset();
    m_targets.clear();
    m_turnColor = Pieces::White;
}

void Controller::handleGameOutCome(Validator::GameOutcome outCome)
{
    switch(outCome) {
    case Validator::Ongoing:
        return;
    case Validator::CheckMate:
        emit gameOver(QLatin1StringView(
            "Checkmate! %1 wins. Would you like to start a new game?")
            .arg(m_turnColor == Pieces::White ? "White" : "Black"));
        return;
    case Validator::StaleMate:
        emit gameOver("Stalemate! Would you like to start a new game?");
        return;
    case Validator::DrawByRepetition:
        emit gameOver("Draw by repitition! "
                      "Would you like to start a new game?");
        return;
    case Validator::DrawBy50MoveRule:
        emit gameOver("Draw by the rule of fifty moves! "
                      "Would you like to start a new game?");
        return;
    }
}
