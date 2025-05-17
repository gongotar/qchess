#include "moveexecutor.h"
#include "board.h"
#include "gamestate.h"
#include "square.h"
#include "pieces.h"

MoveExecutor::MoveExecutor(Board* board): m_board(board) {}

void MoveExecutor::operator()(Square *from, Square *to, GameState states[]) const noexcept
{
    const QChar piece = from->piece();
    const Pieces::Color turnColor = Pieces::pieceColor(piece);
    GameState& state = states[turnColor];
    const bool isKing = from == state.m_king;

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
    else if (piece == Pieces::BlackPawn) {
        if (to->row() - from->row() == 2)
            states[Pieces::White].m_enPassantTarget = m_board->at(to->row() - 1, to->col());
        else if (to->col() - from->col() != 0 && to->piece() == Pieces::Empty) // en passant
            m_board->at(to->row() - 1, to->col())->setPiece(Pieces::Empty);
    }
    else if (piece == Pieces::WhitePawn) {
        if (to->row() - from->row() == -2)
            states[Pieces::Black].m_enPassantTarget = m_board->at(to->row() + 1, to->col());
        else if (to->col() - from->col() != 0 && to->piece() == Pieces::Empty) // en passant
            m_board->at(to->row() + 1, to->col())->setPiece(Pieces::Empty);
    }

    if (piece == Pieces::WhiteRook && from->col() == 0)
        state.m_queenSideCastleRight = false;
    else if (piece == Pieces::BlackRook && from->col() == 7)
        state.m_kingSideCastleRight = false;
    if (isKing) {
        state.m_king = to;
        state.m_kingSideCastleRight = false;
        state.m_queenSideCastleRight = false;
    }
    state.m_enPassantTarget = nullptr;

    to->setPiece(piece);
    from->setPiece(Pieces::Empty);
}
