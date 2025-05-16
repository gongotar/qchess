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
    void setPiece(const QChar &piece)
    {
        m_piece = piece;
        emit pieceChanged();
    }

    void setPieceQuitely(const QChar &piece)
    {
        m_piece = piece;
    }

    bool isLegalDestination() const { return m_isLegalDestination; }
    void setLegalDestination(bool value) {
        if (m_isLegalDestination != value) {
            m_isLegalDestination = value;
            emit legalDestinationChanged();
        }
    }

    bool highlight() const { return m_highlight; }
    void setHighlight(bool value) {
        if (m_highlight != value) {
            m_highlight = value;
            emit highlightChanged();
        }
    }

    bool isSelected() const { return m_isSelected; }
    void setIsSelected(bool value) {
        if (m_isSelected != value) {
            m_isSelected = value;
            emit isSelectedChanged();
        }
    }

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
