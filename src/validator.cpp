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

#include "board.h"
#include "gamestate.h"
#include "pieces.h"
#include "square.h"
#include "validator.h"

#define APPEND_IF_NOT_IN_CHECK(target)                    \
do {                                                      \
    if (!isInCheck(from, target, state.m_king)) {         \
        moves.insert(target);                             \
        if constexpr (stopAtFirst)                        \
            return moves;                                 \
    }                                                     \
} while (0)

namespace {

    bool canThreaten(const QChar& piece, const std::pair<int, int>& dir, int distance) noexcept
    {
        switch (piece.unicode()) {
        case Pieces::WhitePawnCode:
            return dir.first == 1 && dir.second != 0 && distance == 1;
        case Pieces::BlackPawnCode:
            return dir.first == -1 && dir.second != 0 && distance == 1;
        case Pieces::BlackBishopCode:
        case Pieces::WhiteBishopCode:
            return dir.first != 0 && dir.second != 0;
        case Pieces::BlackRookCode:
        case Pieces::WhiteRookCode:
            return dir.first == 0 || dir.second == 0;
        case Pieces::BlackKingCode:
        case Pieces::WhiteKingCode:
            return distance == 1;
        case Pieces::BlackQueenCode:
        case Pieces::WhiteQueenCode:
            return true;
        default:
            return false;
        }
    }

    class Simulator
    {
        Square* m_from;
        Square* m_to;
        const Board& m_board;
        Square* m_king;
        const QChar m_to_piece;
    public:
        Simulator(Square* from, Square* to, Square* king, const Board& board):
            m_from(from),
            m_to(to),
            m_king (king),
            m_board(board),
            m_to_piece(to->piece())
        {
            if (from == king)
                m_king = to;
            else if (from->piece() == Pieces::BlackPawn
                     && (to->col() - from->col() != 0 && to->piece() == Pieces::Empty))
                m_board.at(to->row() - 1, to->col())->setPieceQuitely(Pieces::Empty);
            else if (from->piece() == Pieces::WhitePawn
                     && (to->col() - from->col() != 0 && to->piece() == Pieces::Empty))
                m_board.at(to->row() + 1, to->col())->setPieceQuitely(Pieces::Empty);
            to->setPieceQuitely(from->piece());
            if (from != to)
                from->setPieceQuitely(Pieces::Empty);
        }

        const Square* king() const noexcept
        {
            return m_king;
        }

        ~Simulator() noexcept
        {
            m_from->setPieceQuitely(m_to->piece());
            m_to->setPieceQuitely(m_to_piece);
            if (m_from->piece() == Pieces::BlackPawn
                     && (m_to->col() - m_from->col() != 0 && m_to->piece() == Pieces::Empty))
                m_board.at(m_to->row() - 1, m_to->col())->setPieceQuitely(Pieces::WhitePawn);
            else if (m_from->piece() == Pieces::WhitePawn
                     && (m_to->col() - m_from->col() != 0 && m_to->piece() == Pieces::Empty))
                m_board.at(m_to->row() + 1, m_to->col())->setPieceQuitely(Pieces::BlackPawn);
        }
    };
}

// draw by repitition
// timer
// format choice
// color choice
// flip board
// taken pieces
// count non taken piees, optimze mate calculation
// highlight when trying to move in check
// undo
// sounds

bool Validator::isInCheck(const Square *king) const noexcept
{
    const int kingRow = king->row();
    const int kingCol = king->col();
    const Pieces::Color kingColor = Pieces::pieceColor(king->piece());
    static const std::vector<std::pair<int, int>> directions = {
        { -1,  0 }, // up
        {  1,  0 }, // down
        {  0, -1 }, // left
        {  0,  1 }, // right
        { -1, -1 }, // up-left
        { -1,  1 }, // up-right
        {  1, -1 }, // down-left
        {  1,  1 }  // down-right
    };

    for (const auto& dir : directions) {
        int r = kingRow + dir.first;
        int c = kingCol + dir.second;
        int distance = 1;
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            const QChar piece = m_board.at(r, c)->piece();
            if (piece != Pieces::Empty) {
                if (Pieces::pieceColor(piece) != kingColor)
                    if (canThreaten(piece, dir, distance))
                        return true;
                break;
            }
            r += dir.first;
            c += dir.second;
            distance++;
        }
    }

    // Check knights
    static const std::vector<std::pair<int, int>> knightMoves = {
        { -2, -1 }, { -2,  1 }, { -1, -2 }, { -1,  2 },
        {  1, -2 }, {  1,  2 }, {  2, -1 }, {  2,  1 }
    };

    const QChar opponentKnight = kingColor == Pieces::White?
                                     Pieces::BlackKnight: Pieces::WhiteKnight;

    for (const auto& move : knightMoves) {
        const int r = kingRow + move.first;
        const int c = kingCol + move.second;
        if (r >= 0 && r < 8 && c >= 0 && c < 8
            && m_board.at(r, c)->piece() == opponentKnight)
            return true;
    }

    return false;
}

bool Validator::isInCheck (Square* from, Square* to, Square* king) const noexcept
{
    Simulator sim(from, to, king, m_board);
    return isInCheck(sim.king());
}

bool Validator::isCastlePathInCheck(Square *king, int direction) const noexcept
{
    const Square* rook = m_board.at(king->row(), direction > 0? 7: 0);
    const int col1 = rook->col();
    const int col2 = king->col();
    const int step = col2 > col1? 1:-1;
    for (int i = col1 + step; i != col2; i+=step) {
        Square* movedKing = m_board.at(king->row(), i);
        if (isInCheck(king, movedKing, king))
            return true;
    }
    return false;
}

bool Validator::isPathClear(const Square *from, const Square *to) const noexcept
{
    const int dr = to->row() - from->row();
    const int dc = to->col() - from->col();

    const int stepR = (dr == 0) ? 0 : (dr > 0 ? 1 : -1);
    const int stepC = (dc == 0) ? 0 : (dc > 0 ? 1 : -1);

    int r = from->row() + stepR;
    int c = from->col() + stepC;

    while (r != to->row() || c != to->col()) {
        const Square* intermediate = m_board.at(r, c);
        if (intermediate->piece() != Pieces::Empty)
            return false;
        r += stepR;
        c += stepC;
    }

    return true;
}

bool Validator::hasLegalTargets(const GameState &state) const
{
    const Pieces::Color color = Pieces::pieceColor(state.m_king->piece());
    for (int row = 0; row < 8; ++row)
        for (int col = 0; col < 8; ++col)
            if (Square* sq = m_board.at(row, col);
                Pieces::pieceColor(sq->piece()) == color)
                if (!getLegalTargets<true>(sq, state).empty())
                    return true;
    return false;
}

bool Validator::isRepitition(const GameState& myState, const GameState& opponentState) const
{
    return false;
}

template <bool stopAtFirst>
QSet<Square *> Validator::getLegalTargets(Square *from, const GameState& state) const
{
    QSet<Square*> moves;

    const int row = from->row();
    const int col = from->col();
    const QChar piece = from->piece();
    const Pieces::Color color = Pieces::pieceColor(piece);
    const bool isWhite = color == Pieces::White;

    auto isInsideBoard = [](int r, int c) {
        return r >= 0 && r < 8 && c >= 0 && c < 8;
    };

    auto canMoveTo = [this, &isInsideBoard, color](int r, int c) {
        if (!isInsideBoard(r, c))
            return false;
        Square* to = m_board.at(r, c);
        return to->piece() == Pieces::Empty || Pieces::pieceColor(to->piece()) != color;
    };

    switch (piece.unicode()) {
    case Pieces::WhitePawnCode:
    case Pieces::BlackPawnCode:
    {
        const int direction = isWhite ? -1 : 1;
        const int startRow = isWhite ? 6 : 1;

        // One forward
        if (Square *target = m_board.at(row + direction, col);
            isInsideBoard(row + direction, col) && target->piece() == Pieces::Empty) {
            APPEND_IF_NOT_IN_CHECK(target);
        }

        // Two forward
        if (row == startRow &&
            m_board.at(row + direction, col)->piece() == Pieces::Empty &&
            m_board.at(row + 2 * direction, col)->piece() == Pieces::Empty) { // Two forward
            APPEND_IF_NOT_IN_CHECK(m_board.at(row + 2 * direction, col));
        }
        else if (state.m_enPassantTarget
                 && std::abs(col - state.m_enPassantTarget->col()) == 1
                 && row == (7 + direction) / 2) { // en passant
            APPEND_IF_NOT_IN_CHECK(state.m_enPassantTarget);
        }

        // Capture diagonally
        for (const int dc : {-1, 1}) {
            const int newCol = col + dc;
            const int newRow = row + direction;
            if (isInsideBoard(newRow, newCol)) {
                Square* target = m_board.at(newRow, newCol);
                if (target->piece() != Pieces::Empty && Pieces::pieceColor(target->piece()) != color) {
                    APPEND_IF_NOT_IN_CHECK(target);
                }
            }
        }

        break;
    }
    case Pieces::WhiteKnightCode:
    case Pieces::BlackKnightCode:
    {
        const int knightMoves[8][2] = {
            {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
            {-2, -1}, {-1, -2}, {1, -2}, {2, -1}
        };
        for (auto [dr, dc] : knightMoves) {
            int r = row + dr, c = col + dc;
            if (canMoveTo(r, c)) {
                APPEND_IF_NOT_IN_CHECK(m_board.at(r, c));
            }
        }
        break;
    }
    case Pieces::WhiteBishopCode:
    case Pieces::BlackBishopCode:
    case Pieces::WhiteRookCode:
    case Pieces::BlackRookCode:
    case Pieces::WhiteQueenCode:
    case Pieces::BlackQueenCode:
    {
        std::vector<std::pair<int, int>> directions;

        if (piece == Pieces::WhiteBishopCode || piece == Pieces::BlackBishopCode
            || piece == Pieces::WhiteQueenCode || piece == Pieces::BlackQueenCode)
            directions.insert(directions.end(), {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}});
        if (piece == Pieces::WhiteRookCode || piece == Pieces::BlackRookCode
            || piece == Pieces::WhiteQueenCode || piece == Pieces::BlackQueenCode)
            directions.insert(directions.end(), {{-1, 0}, {1, 0}, {0, -1}, {0, 1}});

        for (const auto [dr, dc] : directions) {
            for (int step = 1; step < 8; ++step) {
                const int r = row + dr * step;
                const int c = col + dc * step;
                if (!isInsideBoard(r, c))
                    break;
                Square* target = m_board.at(r, c);
                if (target->piece() == Pieces::Empty) {
                    APPEND_IF_NOT_IN_CHECK(target);
                }
                else {
                    if (Pieces::pieceColor(target->piece()) != color) {
                        APPEND_IF_NOT_IN_CHECK(target);
                    }
                    break;
                }
            }
        }
        break;
    }
    case Pieces::WhiteKingCode:
    case Pieces::BlackKingCode:
    {
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0)
                    continue;
                int r = row + dr, c = col + dc;
                if (canMoveTo(r, c)) {
                    APPEND_IF_NOT_IN_CHECK(m_board.at(r, c));
                }
            }
        }

        const bool testCheck = (state.m_kingSideCastleRight ||
                                state.m_queenSideCastleRight)
                               && isInCheck(from);
        if (state.m_kingSideCastleRight && !testCheck &&
            isPathClear(from, m_board.at(row, 7)) &&
            !isCastlePathInCheck(state.m_king, 1)) {
            APPEND_IF_NOT_IN_CHECK(m_board.at(row, 6));
        }
        if (state.m_queenSideCastleRight && !testCheck &&
            isPathClear(from, m_board.at(row, 0)) &&
            !isCastlePathInCheck(state.m_king, -1)) {
            APPEND_IF_NOT_IN_CHECK(m_board.at(row, 2));
        }
        break;
    }
    }

    return moves;
}

Validator::GameOutcome Validator::evaluateGameOutcome(const GameState &myState,
                                                      const GameState& opponentState) const
{
    if (const bool moves = hasLegalTargets(opponentState); !moves) {
        if (isInCheck(opponentState.m_king))
            return GameOutcome::CheckMate;
        return GameOutcome::StaleMate;
    }
    else if (myState.m_noPawnMoveOrCapture == 50)
        return GameOutcome::DrawBy50MoveRule;
    else if (isRepitition(myState, opponentState))
    return GameOutcome::Ongoing;
}

template QSet<Square*>
Validator::getLegalTargets<true>(Square* from, const GameState& state) const;
template QSet<Square*>
Validator::getLegalTargets<false>(Square* from, const GameState& state) const;
