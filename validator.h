#ifndef VALIDATOR_H
#define VALIDATOR_H

class Board;
class Square;

class Validator
{
public:
    Validator(const Board* board): m_board (board) {}
    bool isLegalMove(const Square* fromSquare, const Square* targetSquare);
    bool isInCheck(const Square* king);
private:
    bool isPathClear(const Square* from, const Square* to) const noexcept;

    const Board* m_board;
};

#endif // VALIDATOR_H
