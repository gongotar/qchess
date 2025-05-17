#include "controller.h"
#include "square.h"
#include "board.h"

Controller::Controller(Board *board, QObject *parent) :
    QObject(parent),
    m_board (board),
    m_validator(board),
    m_moveExec(board)
{
    m_states[Pieces::White].m_king = m_board->at(7, 4);
    m_states[Pieces::Black].m_king = m_board->at(0, 4);
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

    Square* selected = m_board->at(row, col);
    GameState& state = m_states[m_turnColor];

    if (!m_from || !m_targets.contains(selected)) {
        if (Pieces::pieceColor(selected->piece()) != m_turnColor)
            return;
        QList <Square*> targets = m_validator.getLegalTargets(selected, state);
        m_targets = m_validator.getNotInCheck(selected, targets, state.m_king);
        m_from = selected;

        for (Square* sq : std::as_const(m_targets))
            sq->setLegalDestination(true);
        selected->setIsSelected(true);
        return;
    }
    else if (m_from) {
        m_moveExec(m_from, selected, m_states);
        m_turnColor = static_cast<Pieces::Color>(1 - static_cast<int>(m_turnColor));
        m_from->setHighlight(true);
        selected->setHighlight(true);
        m_prevMove.emplace(m_from, selected);
    }

    m_from = nullptr;
    m_targets.clear();

}
