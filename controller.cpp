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
}

Controller::Controller(Board *board, QObject *parent) :
    QObject(parent),
    m_board (board),
    m_validator(board)
{
    m_states[Pieces::White].m_king = m_board->at(7, 4);
    m_states[Pieces::Black].m_king = m_board->at(0, 4);
}

void Controller::selectOrMovePiece(int row, int col)
{
    if (row < 0 || row >= 8 || col < 0 || col >= 8)
        return;

    Square* selected = m_board->at(row, col);
    if (m_prevMove) {
        m_prevMove->first->setHighlight(false);
        m_prevMove->second->setHighlight(false);
        m_prevMove.reset();
    }

    if (!m_from) {
        if (Pieces::pieceColor(selected->piece()) != m_turnColor)
            return;
        QList <Square*> targets = m_validator.getLegalTargets(selected, m_states[m_turnColor]);
        m_targets = m_validator.getNotInCheck(selected, targets, m_states[m_turnColor]);
        for (Square* sq : std::as_const(m_targets))
            sq->setLegalDestination(true);
        m_from.emplace(selected);
        selected->setHighlight(true);
        return;
    }
    else if (m_targets.contains(selected)) {
        Square* to = selected;
        Square* from = *m_from;

        if (from->piece() == Pieces::WhiteRook || from->piece() == Pieces::BlackRook) {
            if (from->col() == 0)
                m_states[m_turnColor].m_queenSideCastleRight = false;
            else if (from->col() == 7)
                m_states[m_turnColor].m_kingSideCastleRight = false;
        }

        to->setPiece(from->piece());
        from->setPiece(Pieces::Empty);

        if (from == m_states[m_turnColor].m_king && to->col() - from->col() == 2) {
            Square* rookFrom = m_board->at(from->row(), 7);
            Square* rookTo = m_board->at(from->row(), 5);
            rookTo->setPiece(rookFrom->piece());
            rookFrom->setPiece(Pieces::Empty);
        }
        else if (from == m_states[m_turnColor].m_king && to->col() - from->col() == -2) {
            Square* rookFrom = m_board->at(from->row(), 0);
            Square* rookTo = m_board->at(from->row(), 3);
            rookTo->setPiece(rookFrom->piece());
            rookFrom->setPiece(Pieces::Empty);
        }
        //else if (moveType == Validator::EnPassant) {

        //}

        if (from == m_states[m_turnColor].m_king) {
            m_states[m_turnColor].m_king = to;
            m_states[m_turnColor].m_kingSideCastleRight = false;
            m_states[m_turnColor].m_queenSideCastleRight = false;
        }
        m_turnColor = static_cast<Pieces::Color>(1 - static_cast<int>(m_turnColor));
        to->setHighlight(true);
        m_prevMove.emplace(from, to);
    }

    for (Square* sq : std::as_const(m_targets))
        sq->setLegalDestination(false);
    m_from.reset();
    m_targets.clear();

}
