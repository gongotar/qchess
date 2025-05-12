#ifndef VALIDATOR_H
#define VALIDATOR_H

class Board;
class Square;
class GameState;

class Validator
{
public:

    enum MoveType
    {
        NormalMove,
        IllegalMove,
        CastleQueenSide,
        CastleKingSide,
        EnPassant
    };

    Validator(const Board* board): m_board (board) {}
    MoveType isLegalMove(const Square* fromSquare, const Square* targetSquare, const GameState&) noexcept;
    bool isInCheck(Square* from, Square* to, Square* king) noexcept;
    bool isCastlePathInCheck(Square* king, MoveType moveType) noexcept;
private:
    bool isPathClear(const Square* from, const Square* to) const noexcept;

    const Board* m_board;
};

#endif // VALIDATOR_H
