#include "validator.h"
#include "square.h"
#include "board.h"
#include "pieces.h"

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
}

bool Validator::isLegalMove(const Square *fromSquare, const Square *targetSquare)
{
    const int dr = targetSquare->row() - fromSquare->row();
    const int dc = targetSquare->col() - fromSquare->col();

    const int absDr = std::abs(dr);
    const int absDc = std::abs(dc);

    switch (fromSquare->piece().unicode()) {
    case Pieces::BlackPawnCode:
    {
        const bool emptyTarget = targetSquare->piece() == Pieces::Empty;
        if (dc == 0 && dr == 1 && emptyTarget)
            return true;
        if (dc == 0 && dr == 2
            && fromSquare->row() == 1
            && emptyTarget
            && isPathClear(fromSquare, targetSquare))
            return true;
        if (absDc == 1 && dr == 1 && !emptyTarget)
            return true;
        return false;
    }
    case Pieces::WhitePawnCode:
    {
        const bool emptyTarget = targetSquare->piece() == Pieces::Empty;
        if (dc == 0 && dr == -1 && emptyTarget)
            return true;
        if (dc == 0 && dr == -2
            && fromSquare->row() == 6
            && emptyTarget
            && isPathClear(fromSquare, targetSquare))
            return true;
        if (absDc == 1 && dr == -1 && !emptyTarget)
            return true;
        return false;
    }
    case Pieces::BlackKnightCode:
    case Pieces::WhiteKnightCode:
        return (absDr == 2 && absDc == 1) || (absDr == 1 && absDc == 2);

    case Pieces::BlackBishopCode:
    case Pieces::WhiteBishopCode:
        return absDr == absDc && isPathClear(fromSquare, targetSquare);
        ;

    case Pieces::BlackRookCode:
    case Pieces::WhiteRookCode:
        return (dr == 0 || dc == 0) && (dr != 0 || dc != 0)
               && isPathClear(fromSquare, targetSquare);

    case Pieces::BlackQueenCode:
    case Pieces::WhiteQueenCode:
        return (absDr == absDc) || (dr == 0 || dc == 0)
                                       && isPathClear(fromSquare, targetSquare);

    case Pieces::BlackKingCode:
    case Pieces::WhiteKingCode:
        return absDr <= 1 && absDc <= 1 && (dr != 0 || dc != 0);

    default:
        return false;
    }

    // castle (if king / rook not moved)
    // en pasaunt!
    // pawn promotion
    // show selection
    // show move
    // timer
    // format choice
    // color choice
    // flip board
    // taken pieces
    // highlight when tryying to move in check
    // draw by repitition
    // draw by 50 moves
    // mate
    // stale mate
}

bool Validator::isInCheck (const Square* king)
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
            const QChar piece = m_board->at(r, c)->piece();
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
            && m_board->at(r, c)->piece() == opponentKnight)
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
        const Square* intermediate = m_board->at(r, c);
        if (intermediate->piece() != Pieces::Empty)
            return false;
        r += stepR;
        c += stepC;
    }

    return true;
}
