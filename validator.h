#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <QList>
#include <QSet>

class Board;
class Square;
class GameState;

class Validator
{
public:
    Validator(const Board* board): m_board (board) {}
    bool isInCheck(Square* from, Square* to, Square* king) const noexcept;
    bool isCastlePathInCheck(Square* king, int direction) const noexcept;
    QList<Square*> getLegalTargets(Square* from, const GameState& state) const;
    QSet<Square*> getNotInCheck(Square* from, const QList<Square*>& targets, const GameState&) const;
private:
    bool isPathClear(const Square* from, const Square* to) const noexcept;

    const Board* m_board;
};

#endif // VALIDATOR_H
