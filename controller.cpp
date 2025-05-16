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
        m_from->setIsSelected(false);

    Square* to = m_board->at(row, col);
    GameState& state = m_states[m_turnColor];

    if (!m_from || !m_targets.contains(to)) {
        if (Pieces::pieceColor(to->piece()) != m_turnColor)
            return;
        QList <Square*> targets = m_validator.getLegalTargets(to, state);
        m_targets = m_validator.getNotInCheck(to, targets, state.m_king);
        m_from = to;

        for (Square* sq : std::as_const(m_targets))
            sq->setLegalDestination(true);
        to->setIsSelected(true);
        return;
    }
    else if (m_from) {
        const bool isKing = m_from == state.m_king;
        const QChar piece = m_from->piece();
        if (piece == Pieces::WhiteRook && m_from->col() == 0)
            state.m_queenSideCastleRight = false;
        else if (piece == Pieces::BlackRook && m_from->col() == 7)
            state.m_kingSideCastleRight = false;

        if (isKing && col - m_from->col() == 2) {
            Square* rookFrom = m_board->at(m_from->row(), 7);
            Square* rookTo = m_board->at(m_from->row(), 5);
            rookTo->setPiece(rookFrom->piece());
            rookFrom->setPiece(Pieces::Empty);
        }
        else if (isKing && col - m_from->col() == -2) {
            Square* rookFrom = m_board->at(m_from->row(), 0);
            Square* rookTo = m_board->at(m_from->row(), 3);
            rookTo->setPiece(rookFrom->piece());
            rookFrom->setPiece(Pieces::Empty);
        }
        else if (piece == Pieces::BlackPawn) {
            if (row - m_from->row() == 2)
                m_states[Pieces::White].m_enPassantTarget = m_board->at(row - 1, col);
            else if (col - m_from->col() != 0 && to->piece() == Pieces::Empty) // en passant
                m_board->at(row - 1, col)->setPiece(Pieces::Empty);
        }
        else if (piece == Pieces::WhitePawn) {
            if (row - m_from->row() == -2)
                m_states[Pieces::Black].m_enPassantTarget = m_board->at(row + 1, col);
            else if (col - m_from->col() != 0 && to->piece() == Pieces::Empty) // en passant
                m_board->at(row + 1, col)->setPiece(Pieces::Empty);
        }

        if (isKing) {
            state.m_king = to;
            state.m_kingSideCastleRight = false;
            state.m_queenSideCastleRight = false;
        }

        to->setPiece(piece);
        m_from->setPiece(Pieces::Empty);

        state.m_enPassantTarget = nullptr;
        m_turnColor = static_cast<Pieces::Color>(1 - static_cast<int>(m_turnColor));
        m_from->setHighlight(true);
        to->setHighlight(true);
        m_prevMove.emplace(m_from, to);
    }

    m_from = nullptr;
    m_targets.clear();

}
