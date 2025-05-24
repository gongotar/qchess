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

#ifndef BOARD_H
#define BOARD_H

#include <QAbstractListModel>
#include <QObject>

class Square;

class Board: public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles { RowRole = Qt::UserRole + 1, ColRole, PieceRole, IsLegalDestinationRole, HighlightRole, IsSelectedRole };

    Board(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const noexcept override {return 64;}
    QVariant data(const QModelIndex &index, int role) const noexcept override;
    QHash<int, QByteArray> roleNames() const noexcept override;


    Square* at(int row, int col) const noexcept {
        return m_squares[row][col];
    }

private:
    void init();
    Square* m_squares[8][8];
};

#endif // BOARD_H
