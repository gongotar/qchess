#ifndef GAMESTATE_H
#define GAMESTATE_H

class Square;

struct GameState
{
    Square* m_king;
    bool m_kingSideCastleRight = true;
    bool m_queenSideCastleRight = true;
};

#endif // GAMESTATE_H
