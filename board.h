#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QAbstractListModel>

class Square;

class Board: public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles { RowRole = Qt::UserRole + 1, ColRole, PieceRole, IsLegalDestinationRole, HighlightRole };

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
