#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QSet>
#include "validator.h"
#include "pieces.h"
#include "gamestate.h"

class Board;

class Controller: public QObject
{
    Q_OBJECT
public:
    Controller(Board* board, QObject *parent = nullptr);
    Q_INVOKABLE void selectOrMovePiece(int row, int col);
private:

    const Board* m_board;
    Validator m_validator;
    std::optional<Square*> m_from;
    std::optional<std::pair<Square*, Square*>> m_prevMove;
    QSet <Square*> m_targets;
    GameState m_states[2];
    Pieces::Color m_turnColor = Pieces::Color::White;
};

#endif // CONTROLLER_H
