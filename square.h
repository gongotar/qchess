#ifndef SQUARE_H
#define SQUARE_H

#include <QObject>

class Square: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int row READ row CONSTANT)
    Q_PROPERTY(int col READ col CONSTANT)
    Q_PROPERTY(QChar piece READ piece WRITE setPiece NOTIFY pieceChanged)

public:
    Square(int r, int c, const QChar &p, QObject *parent = nullptr): QObject(parent),
        m_row(r),
        m_col(c),
        m_piece(p) {
    }

    int row() const noexcept {return m_row;}
    int col() const noexcept {return m_col;}
    QChar piece() const noexcept {return m_piece;}
    void setPiece(const QChar &piece)
    {
        m_piece = piece;
        emit pieceChanged();
    }

    void setPieceQuitely(const QChar &piece)
    {
        m_piece = piece;
    }

signals:
    void pieceChanged();

private:
    const int m_row;
    const int m_col;
    QChar m_piece;
};

#endif // SQUARE_H
