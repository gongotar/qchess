#include "controller.h"
#include "square.h"
#include "board.h"

namespace {
enum Action
{
    Reset,
    SelectSource,
    SelectTarget,
};

Action selectionAction(const std::optional<Square*>& m_prev, const Square* current, Pieces::Color turnColor) noexcept
{
    const Pieces::Color currentColor = Pieces::pieceColor(current->piece());
    if (!m_prev) {
        if(turnColor != currentColor)
            return Reset;
        else
            return SelectSource;
    }
    if (currentColor == turnColor)
        return Reset;
    return SelectTarget;
}

class SimulatorScope
{
    Square* m_from;
    Square* m_to;
    Square** m_king;
    const QChar m_from_piece, m_to_piece;
public:
    SimulatorScope(Square* from, Square* to, Square** king):
        m_from(from),
        m_from_piece(from->piece()),
        m_to(to),
        m_to_piece(to->piece()),
        m_king (king)
    {
        if (from == *king)
            *king = to;
        to->m_piece = from->piece();
        from->m_piece = Pieces::Empty;
    }
    const Square* king() const noexcept
    {
        return *m_king;
    }
    ~SimulatorScope()
    {
        m_to->m_piece = m_to_piece;
        m_from->m_piece = m_from_piece;
        if (*m_king == m_to)
            *m_king = m_from;
    }
};

[[nodiscard]] SimulatorScope simulateMove(Square* from, Square* to, Square** king) noexcept
{
    return {from, to, king};
}

}

Controller::Controller(Board *board, QObject *parent) :
    QObject(parent),
    m_board (board),
    m_validator(board),
    m_whiteKing(m_board->at(7, 4)),
    m_blackKing(m_board->at(0, 4))
{
}

void Controller::selectOrMovePiece(int row, int col)
{
    if (row < 0 || row >= 8 || col < 0 || col >= 8)
        return;

    bool move = false;
    Square* currentSquare = m_board->at(row, col);
    if (Action act = selectionAction(m_prevSelected, currentSquare, m_turnColor);
        act == Reset)
        m_prevSelected.reset();
    else if (act == SelectSource)
        m_prevSelected.emplace(currentSquare);
    else if (!m_validator.isLegalMove(*m_prevSelected, currentSquare))
        m_prevSelected.reset();
    else if (SimulatorScope sim = simulateMove(*m_prevSelected,
                                               currentSquare,
                                               m_turnColor == Pieces::White? &m_whiteKing: &m_blackKing);
             m_validator.isInCheck(sim.king()))
        m_prevSelected.reset();
    else
        move = true;

    if (!move)
        return;

    currentSquare->setPiece(m_prevSelected.value()->piece());
    m_prevSelected.value()->setPiece(Pieces::Empty);
    Square** king = m_turnColor == Pieces::White? &m_whiteKing: &m_blackKing;
    if (*m_prevSelected == *king)
        *king = currentSquare;
    m_prevSelected.reset();
    m_turnColor = static_cast<Pieces::Color>(1 - static_cast<int>(m_turnColor));
}

