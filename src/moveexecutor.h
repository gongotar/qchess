#ifndef MOVEEXECUTOR_H
#define MOVEEXECUTOR_H

class Board;
class Square;
class GameState;

class MoveExecutor
{
public:
    MoveExecutor(Board* board);
    void operator() (Square* from, Square* to, GameState states[]) const noexcept;

    Board* m_board;
};

#endif // MOVEEXECUTOR_H
