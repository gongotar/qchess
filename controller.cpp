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

    Validator::MoveType moveType;
    Square* to = m_board->at(row, col);
    if (Action act = selectionAction(m_from, to, m_turnColor);
        act == Reset) {
        m_from.reset();
        return;
    }
    else if (act == SelectSource) {
        m_from.emplace(to);
        return;
    }
    else if (moveType = m_validator.isLegalMove(*m_from, to, m_states[m_turnColor]);
               moveType == Validator::IllegalMove) {
        m_from.reset();
        return;
    }
    else if ((moveType == Validator::CastleKingSide || moveType == Validator::CastleQueenSide)
               && m_validator.isCastlePathInCheck(m_from.value(), moveType)) {
        m_from.reset();
        return;
    }
    else if ((moveType == Validator::NormalMove || moveType == Validator::EnPassant)
               && m_validator.isInCheck(*m_from, to, m_states[m_turnColor].m_king)) {
        m_from.reset();
        return;
    }

    Square* from = *m_from;
    if (from == m_states[m_turnColor].m_king) {
        m_states[m_turnColor].m_king = to;
        m_states[m_turnColor].m_kingSideCastleRight = false;
        m_states[m_turnColor].m_queenSideCastleRight = false;
    }

    if (from->piece() == Pieces::WhiteRook || from->piece() == Pieces::BlackRook) {
        if (from->col() == 0)
            m_states[m_turnColor].m_queenSideCastleRight = false;
        else if (from->col() == 7)
            m_states[m_turnColor].m_kingSideCastleRight = false;
    }


    to->setPiece(from->piece());
    from->setPiece(Pieces::Empty);

    if (moveType == Validator::CastleKingSide) {
        Square* rookFrom = m_board->at(from->row(), 7);
        Square* rookTo = m_board->at(from->row(), 5);
        rookTo->setPiece(rookFrom->piece());
        rookFrom->setPiece(Pieces::Empty);
    }
    else if (moveType == Validator::CastleQueenSide) {
        Square* rookFrom = m_board->at(from->row(), 0);
        Square* rookTo = m_board->at(from->row(), 3);
        rookTo->setPiece(rookFrom->piece());
        rookFrom->setPiece(Pieces::Empty);
    }
    else if (moveType == Validator::EnPassant) {

    }

    m_from.reset();
    m_turnColor = static_cast<Pieces::Color>(1 - static_cast<int>(m_turnColor));
}

