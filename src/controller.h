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
#include "validator.h"

#include <QObject>
#include <qqmlintegration.h>
#include <QSet>
#include <QString>

class Controller: public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
    Q_PROPERTY(Board* board READ board CONSTANT)
    Q_PROPERTY(bool flipped READ flipped WRITE setFlipped NOTIFY flippedChanged)
    Q_PROPERTY(QString whiteCapturePieces READ whiteCapturePieces NOTIFY capturesChanged)
    Q_PROPERTY(int whiteCaptureValue READ whiteCaptureValue NOTIFY capturesChanged)
    Q_PROPERTY(QString blackCapturePieces READ blackCapturePieces NOTIFY capturesChanged)
    Q_PROPERTY(int blackCaptureValue READ blackCaptureValue NOTIFY capturesChanged)

public:
    Controller(QObject *parent = nullptr);
    Q_INVOKABLE void selectOrMovePiece(int row, int col);
    Q_INVOKABLE void promotePawnTo(int row, int col, const QChar& piece);
    Q_INVOKABLE void restartGame(bool white = true);
    Board* board() const noexcept {return const_cast<Board*>(&m_board);}
    bool flipped() const noexcept {return m_flipped;}
    void setFlipped(bool val) {if (val != m_flipped) flipBoard();}
    QString whiteCapturePieces() const noexcept;
    int whiteCaptureValue() const noexcept;
    QString blackCapturePieces() const noexcept;
    int blackCaptureValue() const noexcept;
signals:
    void promotePawn(int row, int col, const QList<QChar>& choices);
    void gameOver(const QString message);
    void flippedChanged();
    void capturesChanged();
private:
    void flipBoard();
    void handleGameOutCome(Validator::GameOutcome outCome);

    Board m_board;
    const Validator m_validator;
    const MoveExecutor m_moveExec;
    Square* m_from;
    std::optional<std::pair<Square*, Square*>> m_prevMove;
    QSet <Square*> m_targets;
    GameState m_state;
    bool m_flipped = false;
};

#endif // CONTROLLER_H
