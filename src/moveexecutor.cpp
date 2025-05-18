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


#include "moveexecutor.h"
#include "board.h"
#include "gamestate.h"
#include "square.h"
#include "pieces.h"

MoveExecutor::MoveExecutor(Board* board): m_board(board) {}

void MoveExecutor::operator()(Square *from, Square *to, GameState states[]) const noexcept
{
    const QChar piece = from->piece();
    const Pieces::Color turnColor = Pieces::pieceColor(piece);
    GameState& state = states[turnColor];
    const bool isKing = from == state.m_king;

    if (isKing && to->col() - from->col() == 2) {
        Square* rookFrom = m_board->at(from->row(), 7);
        Square* rookTo = m_board->at(from->row(), 5);
        rookTo->setPiece(rookFrom->piece());
        rookFrom->setPiece(Pieces::Empty);
    }
    else if (isKing && to->col() - from->col() == -2) {
        Square* rookFrom = m_board->at(from->row(), 0);
        Square* rookTo = m_board->at(from->row(), 3);
        rookTo->setPiece(rookFrom->piece());
        rookFrom->setPiece(Pieces::Empty);
    }
    else if (piece == Pieces::BlackPawn) {
        if (to->row() - from->row() == 2)
            states[Pieces::White].m_enPassantTarget = m_board->at(to->row() - 1, to->col());
        else if (to->col() - from->col() != 0 && to->piece() == Pieces::Empty) // en passant
            m_board->at(to->row() - 1, to->col())->setPiece(Pieces::Empty);
    }
    else if (piece == Pieces::WhitePawn) {
        if (to->row() - from->row() == -2)
            states[Pieces::Black].m_enPassantTarget = m_board->at(to->row() + 1, to->col());
        else if (to->col() - from->col() != 0 && to->piece() == Pieces::Empty) // en passant
            m_board->at(to->row() + 1, to->col())->setPiece(Pieces::Empty);
    }

    if (piece == Pieces::WhiteRook && from->col() == 0)
        state.m_queenSideCastleRight = false;
    else if (piece == Pieces::BlackRook && from->col() == 7)
        state.m_kingSideCastleRight = false;
    if (isKing) {
        state.m_king = to;
        state.m_kingSideCastleRight = false;
        state.m_queenSideCastleRight = false;
    }
    state.m_enPassantTarget = nullptr;

    to->setPiece(piece);
    from->setPiece(Pieces::Empty);
}
