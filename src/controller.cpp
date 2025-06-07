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
#include <algorithm>

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

int captureValue(QChar p) noexcept
{
    switch (ushort(p.unicode())) {
    case Pieces::WhiteQueenCode:
    case Pieces::BlackQueenCode:
        return 5;
    case Pieces::WhiteRookCode:
    case Pieces::BlackRookCode:
        return 4;
    case Pieces::WhiteBishopCode:
    case Pieces::BlackBishopCode:
        return 3;
    case Pieces::WhiteKnightCode:
    case Pieces::BlackKnightCode:
        return 2;
    case Pieces::WhitePawnCode:
    case Pieces::BlackPawnCode:
        return 1;
    default:
        return 0;
    }
}

QString sortedCaptures(const QString& list)
{
    QString s = list;
    std::sort(s.begin(), s.end(), [](const QChar& a, const QChar& b){
        return captureValue(a) > captureValue(b);
    });
    return s;
}
}

Controller::Controller(QObject *parent) :
    QObject(parent),
    m_board(this),
    m_validator(m_board),
    m_moveExec(m_board)
{
    m_state.m_white.m_king = m_board.at(7, 4);
    m_state.m_black.m_king = m_board.at(0, 4);
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
    PlayerState& state = m_state.playerState();

    if (!m_from || !m_targets.contains(selected)) {
        if (Pieces::pieceColor(selected->piece()) != m_state.m_turnColor)
            return;
        m_targets = m_validator.getLegalTargets(selected, state, m_flipped);
        m_from = selected;

        for (Square* sq : std::as_const(m_targets))
            sq->setLegalDestination(true);
        selected->setIsSelected(true);
        return;
    }
    else if (m_from) {
        const int prevCapCount = state.m_captures.size();
        m_moveExec(m_from, selected, m_state, m_flipped);
        if (state.m_captures.size() != prevCapCount)
            emit capturesChanged();

        if (const Square* p = state.m_promotedPawnSquare; p)
            emit promotePawn(p->row(), p->col(),
                             promotionChoices(m_state.m_turnColor));
        // update ui
        m_from->setHighlight(true);
        selected->setHighlight(true);
        m_prevMove.emplace(m_from, selected);

        handleGameOutCome(m_validator.evaluateGameOutcome(m_state, m_flipped));

        m_state.switchTurn();
    }
    m_from = nullptr;
    m_targets.clear();
}

void Controller::promotePawnTo(int row, int col, const QChar &piece) {
    m_board.at(row, col)->setPiece(piece);
}

void Controller::restartGame(bool white)
{
    m_board.resetBoard();
    m_from = nullptr;
    m_prevMove.reset();
    m_targets.clear();
    m_flipped = false;

    m_state = GameState{};
    m_state.m_white.m_king = m_board.at(7, 4);
    m_state.m_black.m_king = m_board.at(0, 4);
    emit capturesChanged();
    if (!white) {
        flipBoard();
        m_state.switchTurn();
    }
}

void Controller::flipBoard()
{
    QChar tmpPiece;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 8; ++col) {
            Square* up = m_board.at(row,col);
            Square* down = m_board.at(7 - row,col);
            tmpPiece = up->piece();
            up->setPiece(down->piece());
            down->setPiece(tmpPiece);
            // update king
            if (tmpPiece == Pieces::WhiteKing)
                m_state.m_white.m_king = down;
            else if (tmpPiece == Pieces::BlackKing)
                m_state.m_black.m_king = down;

            if (up->piece() == Pieces::WhiteKing)
                m_state.m_white.m_king = up;
            else if (up->piece() == Pieces::BlackKing)
                m_state.m_black.m_king = up;
        }
    }
    auto flip = [this] (Square*& sq) {
        if (sq)
            sq = m_board.at(7 - sq->row(), sq->col());
    };
    // flip stored state
    flip(m_state.m_white.m_enPassantTarget);
    for (std::pair<Square*, Square*>& move: m_state.m_white.m_moves) {
        flip(move.first);
        flip(move.second);
    }
    flip(m_state.m_black.m_enPassantTarget);
    for (std::pair<Square*, Square*>& move: m_state.m_black.m_moves) {
        flip(move.first);
        flip(move.second);
    }
    m_from = nullptr;
    m_prevMove.reset();
    m_targets.clear();
    m_board.clearSelections();
    m_flipped = !m_flipped;
    emit flippedChanged();
}

void Controller::handleGameOutCome(Validator::GameOutcome outCome)
{
    switch(outCome) {
    case Validator::Ongoing:
        return;
    case Validator::CheckMate:
        emit gameOver(QLatin1StringView(
            "Checkmate! %1 wins. Would you like to start a new game?")
            .arg(m_state.m_turnColor == Pieces::White ? "White" : "Black"));
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


QString Controller::whiteCaptures() const noexcept
{
    return sortedCaptures(m_state.m_white.m_captures);
}

QString Controller::blackCaptures() const noexcept
{
    return sortedCaptures(m_state.m_black.m_captures);
}
