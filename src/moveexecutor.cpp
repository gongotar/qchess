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
#include "pieces.h"
#include "square.h"

MoveExecutor::MoveExecutor(const Board& board): m_board(board) {}

void MoveExecutor::operator()(Square *from, Square *to, GameState* states) const noexcept
{
    const QChar piece = from->piece();
    const Pieces::Color color = Pieces::pieceColor(piece);
    GameState& state = states[color];

    const bool isKing = piece == Pieces::WhiteKing || piece == Pieces::BlackKing;
    const bool isRook = piece == Pieces::WhiteRook || piece == Pieces::BlackRook;
    const bool isPawn = piece == Pieces::WhitePawn || piece == Pieces::BlackPawn;
    bool enPassant = false;
    bool promotion = false;

    if (isKing && to->col() - from->col() == 2) {
        Square* rookFrom = m_board.at(from->row(), 7);
        Square* rookTo = m_board.at(from->row(), 5);
        rookTo->setPiece(rookFrom->piece());
        rookFrom->setPiece(Pieces::Empty);
    }
    else if (isKing && to->col() - from->col() == -2) {
        Square* rookFrom = m_board.at(from->row(), 0);
        Square* rookTo = m_board.at(from->row(), 3);
        rookTo->setPiece(rookFrom->piece());
        rookFrom->setPiece(Pieces::Empty);
    }
    else if (isPawn) {
        const int d = piece == Pieces::BlackPawn? 1: -1;
        if (to->row() - from->row() == 2*d)
            states[1 - color].m_enPassantTarget = m_board.at(to->row() - 1*d, to->col());
        else if (enPassant = to->col() - from->col() != 0
                && to->piece() == Pieces::Empty; enPassant) // en passant
            m_board.at(to->row() - 1*d, to->col())->setPiece(Pieces::Empty);
        else if (promotion = to->row() << 1 == (d+1)*7; promotion) // promotion
            state.m_promotedPawnSquare = to;
    }

    if (const QChar targetPiece = to->piece(); targetPiece != Pieces::Empty)
        state.m_captured.emplace(targetPiece);
    else if (enPassant)
        state.m_captured.emplace((color == Pieces::White)? Pieces::BlackPawn:Pieces::WhitePawn);
    else
        state.m_captured.reset();

    state.m_kingSideCastleRight &= !(isKing || (isRook && from->col() == 7));
    state.m_kingSideCastleRight &= !(isKing || (isRook && from->col() == 0));

    if (isKing)
        state.m_king = to;
    if (!promotion)
        state.m_promotedPawnSquare = nullptr;

    if (!isPawn && !state.m_captured)
        state.m_noPawnMoveOrCapture ++;
    else
        state.m_noPawnMoveOrCapture = 0;

    state.m_lastMoves.emplaceBack(from, to);

    to->setPiece(piece);
    from->setPiece(Pieces::Empty);
}
