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

MoveExecutor::MoveResult MoveExecutor::operator()(Square *from, Square *to) const noexcept
{
    MoveResult result;
    const QChar piece = from->piece();
    const bool isKing = piece == Pieces::WhiteKing || piece == Pieces::BlackKing;
    const bool isRook = piece == Pieces::WhiteRook || piece == Pieces::BlackRook;
    const bool isPawn = piece == Pieces::WhitePawn || piece == Pieces::BlackPawn;

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
    else if (isPawn) {
        const int d = piece == Pieces::BlackPawn? 1: -1;
        if (to->row() - from->row() == 2*d)
            result.m_enPassantTarget = m_board->at(to->row() - 1*d, to->col());
        else if (to->col() - from->col() != 0 && to->piece() == Pieces::Empty) // en passant
            m_board->at(to->row() - 1*d, to->col())->setPiece(Pieces::Empty);
        else if (to->row() << 1 == (d+1)*7) // promotion
            result.m_promotedPawnSquare = to;
    }
    to->setPiece(piece);
    from->setPiece(Pieces::Empty);

    result.m_revokeQueenSideCastleRight = isKing || (isRook && from->col() == 0);
    result.m_revokeKingSideCastleRight = isKing || (isRook && from->col() == 7);
    if (isKing)
        result.m_newKingSquare = to;

    return result;
}
