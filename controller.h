#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "validator.h"
#include "pieces.h"

class Board;
class Square;


class Controller: public QObject
{
    Q_OBJECT
public:
    Controller(Board* board, QObject *parent = nullptr);
    Q_INVOKABLE void selectOrMovePiece(int row, int col);

private:

    const Board* m_board;
    Validator m_validator;
    Square* m_whiteKing;
    Square* m_blackKing;
    Pieces::Color m_turnColor = Pieces::Color::White;
    std::optional<Square*> m_prevSelected;
};

#endif // CONTROLLER_H
