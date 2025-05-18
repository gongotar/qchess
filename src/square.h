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

#ifndef SQUARE_H
#define SQUARE_H

#include <QObject>

class Square: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int row READ row CONSTANT)
    Q_PROPERTY(int col READ col CONSTANT)
    Q_PROPERTY(QChar piece READ piece WRITE setPiece NOTIFY pieceChanged)
    Q_PROPERTY(bool isLegalDestination READ isLegalDestination WRITE setLegalDestination NOTIFY legalDestinationChanged)
    Q_PROPERTY(bool highlight READ highlight WRITE setHighlight NOTIFY highlightChanged)
    Q_PROPERTY(bool isSelected READ isSelected WRITE setIsSelected NOTIFY isSelectedChanged)

public:
    Square(int r, int c, const QChar &p, QObject *parent = nullptr): QObject(parent),
        m_row(r),
        m_col(c),
        m_piece(p) {
    }

    int row() const noexcept {return m_row;}
    int col() const noexcept {return m_col;}

    QChar piece() const noexcept {return m_piece;}
    void setPiece(const QChar &piece);
    void setPieceQuitely(const QChar &piece) { m_piece = piece;}

    bool isLegalDestination() const { return m_isLegalDestination; }
    void setLegalDestination(bool value);

    bool highlight() const { return m_highlight; }
    void setHighlight(bool value);

    bool isSelected() const { return m_isSelected; }
    void setIsSelected(bool value);

signals:
    void pieceChanged();
    void legalDestinationChanged();
    void highlightChanged();
    void isSelectedChanged();

private:
    const int m_row;
    const int m_col;
    QChar m_piece;
    bool m_isLegalDestination = false;
    bool m_highlight = false;
    bool m_isSelected = false;
};

#endif // SQUARE_H
