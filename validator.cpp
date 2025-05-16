#include "validator.h"
#include "square.h"
#include "board.h"
#include "pieces.h"
#include "gamestate.h"

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
        Square* m_king;
        const QChar m_to_piece;
    public:
        Simulator(Square* from, Square* to, Square* king):
            m_from(from),
            m_to(to),
            m_king (king),
            m_to_piece(to->piece())
        {
            if (from == king)
                m_king = to;
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
        }
    };
}

// EnPassant!
// pawn promotion
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
// undo

bool Validator::isInCheck (Square* from, Square* to, Square* king) const noexcept
{
    Simulator sim(from, to, king);
    const int kingRow = sim.king()->row();
    const int kingCol = sim.king()->col();
    const Pieces::Color kingColor = Pieces::pieceColor(sim.king()->piece());

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

bool Validator::isCastlePathInCheck(Square *king, int direction) const noexcept
{
    const Square* rook = m_board->at(king->row(), direction > 0? 7: 0);
    const int col1 = rook->col();
    const int col2 = king->col();
    const int step = col2 > col1? 1:-1;
    for (int i = col1 + step; i != col2 + step; i+=step) {
        Square* movedKing = m_board->at(king->row(), i);
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
        const Square* intermediate = m_board->at(r, c);
        if (intermediate->piece() != Pieces::Empty)
            return false;
        r += stepR;
        c += stepC;
    }

    return true;
}

QList<Square *> Validator::getLegalTargets(Square *from, bool kingSideCastleRight, bool queenSideCastleRight) const
{
    QList<Square*> moves;

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
        Square* to = m_board->at(r, c);
        return to->piece() == Pieces::Empty || Pieces::pieceColor(to->piece()) != color;
    };

    switch (piece.unicode()) {
    case Pieces::WhitePawnCode:
    case Pieces::BlackPawnCode:
    {
        const int direction = isWhite ? -1 : 1;
        const int startRow = isWhite ? 6 : 1;

        // One forward
        if (isInsideBoard(row + direction, col) && m_board->at(row + direction, col)->piece() == Pieces::Empty)
            moves.append(m_board->at(row + direction, col));

        // Two forward
        if (row == startRow &&
            m_board->at(row + direction, col)->piece() == Pieces::Empty &&
            m_board->at(row + 2 * direction, col)->piece() == Pieces::Empty)
            moves.append(m_board->at(row + 2 * direction, col));

        // Capture diagonally
        for (const int dc : {-1, 1}) {
            const int newCol = col + dc;
            const int newRow = row + direction;
            if (isInsideBoard(newRow, newCol)) {
                Square* target = m_board->at(newRow, newCol);
                if (target->piece() != Pieces::Empty && Pieces::pieceColor(target->piece()) != color)
                    moves.append(target);
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
            if (canMoveTo(r, c))
                moves.append(m_board->at(r, c));
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
                Square* target = m_board->at(r, c);
                if (target->piece() == Pieces::Empty)
                    moves.append(target);
                else {
                    if (Pieces::pieceColor(target->piece()) != color)
                        moves.append(target);
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
                if (canMoveTo(r, c))
                    moves.append(m_board->at(r, c));
            }
        }

        if (kingSideCastleRight && isPathClear(from, m_board->at(row, 7)))
            moves.append(m_board->at(row, 6));
        if (queenSideCastleRight && isPathClear(from, m_board->at(row, 0)))
            moves.append(m_board->at(row, 2));

        break;
    }
    }

    return moves;
}

QSet<Square*> Validator::getNotInCheck(Square *from, const QList<Square *>& targets, Square *king) const
{
    QSet<Square *> moves;
    moves.reserve(targets.size());
    for (Square* to: targets) {
        if (int diff = to->col() - from->col();
            (from == king && std::abs(diff) == 2 && !isCastlePathInCheck(from, diff)) // castling
            ||
            (((from != king || std::abs(diff) <= 1) && !isInCheck(from, to, king))))
            moves.insert(to);
    }
    return moves;
}
