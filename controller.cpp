#include "controller.h"
#include "square.h"
#include "board.h"

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
    if (m_prevMove) {
        m_prevMove->first->setHighlight(false);
        m_prevMove->second->setHighlight(false);
        m_prevMove.reset();
    }
    for (Square* sq : std::as_const(m_targets))
        sq->setLegalDestination(false);
    if (m_from)
        m_from.value()->setIsSelected(false);

    Square* to = m_board->at(row, col);
    GameState& state = m_states[m_turnColor];

    if (!m_from || !m_targets.contains(to)) {
        if (Pieces::pieceColor(to->piece()) != m_turnColor)
            return;
        QList <Square*> targets = m_validator.getLegalTargets(to,
                                    state.m_kingSideCastleRight, state.m_queenSideCastleRight);
        m_targets = m_validator.getNotInCheck(to, targets, state.m_king);
        m_from.emplace(to);

        for (Square* sq : std::as_const(m_targets))
            sq->setLegalDestination(true);
        to->setIsSelected(true);
        return;
    }
    else if (m_from) {
        Square* from = *m_from;
        const bool isKing = from == state.m_king;
        const QChar piece = from->piece();
        if (piece == Pieces::WhiteRook && from->col() == 0)
            state.m_queenSideCastleRight = false;
        else if (piece == Pieces::BlackRook && from->col() == 7)
            state.m_kingSideCastleRight = false;

        to->setPiece(piece);
        from->setPiece(Pieces::Empty);

        if (isKing && col - from->col() == 2) {
            Square* rookFrom = m_board->at(from->row(), 7);
            Square* rookTo = m_board->at(from->row(), 5);
            rookTo->setPiece(rookFrom->piece());
            rookFrom->setPiece(Pieces::Empty);
        }
        else if (isKing && col - from->col() == -2) {
            Square* rookFrom = m_board->at(from->row(), 0);
            Square* rookTo = m_board->at(from->row(), 3);
            rookTo->setPiece(rookFrom->piece());
            rookFrom->setPiece(Pieces::Empty);
        }

        if (isKing) {
            state.m_king = to;
            state.m_kingSideCastleRight = false;
            state.m_queenSideCastleRight = false;
        }
        m_turnColor = static_cast<Pieces::Color>(1 - static_cast<int>(m_turnColor));
        from->setHighlight(true);
        to->setHighlight(true);
        m_prevMove.emplace(from, to);
    }

    m_from.reset();
    m_targets.clear();

}
