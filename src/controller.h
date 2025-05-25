// MIT License
//
// Copyright (c) 2025 Masoud Jami
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "board.h"
#include "gamestate.h"
#include "moveexecutor.h"
#include "pieces.h"
#include "validator.h"

#include <QObject>
#include <qqmlintegration.h>
#include <QSet>

class Controller: public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
    Q_PROPERTY(Board* board READ board CONSTANT)

public:
    Controller(QObject *parent = nullptr);
    Q_INVOKABLE void selectOrMovePiece(int row, int col);
    Q_INVOKABLE void promotePawnTo(int row, int col, const QChar& piece);
    Q_INVOKABLE void restartGame();
    Board* board() const noexcept {return const_cast<Board*>(&m_board);}

signals:
    void promotePawn(int row, int col, const QList<QChar>& choices);
    void gameOver(const QString message);

private:
    void handleGameOutCome(Validator::GameOutcome outCome);

    Board m_board;
    const Validator m_validator;
    const MoveExecutor m_moveExec;
    Square* m_from;
    std::optional<std::pair<Square*, Square*>> m_prevMove;
    QSet <Square*> m_targets;
    GameState m_states[2];
    Pieces::Color m_turnColor = Pieces::Color::White;
};

#endif // CONTROLLER_H
