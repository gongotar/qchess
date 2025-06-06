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

#include "board.h"
#include "square.h"
#include "pieces.h"

Board::Board(QObject *parent): QAbstractListModel(parent)
{
    resetBoard();
}

void Board::resetBoard()
{
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QChar piece = Pieces::Empty;
            if (row == 1)
                piece = Pieces::BlackPawn;
            else if (row == 6)
                piece = Pieces::WhitePawn;
            else if (row == 0) {
                switch (col) {
                case 0: case 7: piece = Pieces::BlackRook; break;
                case 1: case 6: piece = Pieces::BlackKnight; break;
                case 2: case 5: piece = Pieces::BlackBishop; break;
                case 3: piece = Pieces::BlackQueen; break;
                case 4: piece = Pieces::BlackKing; break;
                }
            }
            else if (row == 7) {
                switch (col) {
                case 0: case 7: piece = Pieces::WhiteRook; break;
                case 1: case 6: piece = Pieces::WhiteKnight; break;
                case 2: case 5: piece = Pieces::WhiteBishop; break;
                case 3: piece = Pieces::WhiteQueen; break;
                case 4: piece = Pieces::WhiteKing; break;
                }
            }
            if (m_squares[row][col] == nullptr) {
                auto square = new Square(row, col, piece, this);
                connectSquareSignals(square);
                m_squares[row][col] = square;
            }
            else {
                m_squares[row][col]->setPiece(piece);
            }
        }
    }
    clearSelections();
}

void Board::clearSelections() noexcept
{
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Square* square = m_squares[row][col];
            square->setHighlight(false);
            square->setIsSelected(false);
            square->setLegalDestination(false);
        }
    }
}

void Board::connectSquareSignals(Square* square)
{
    const int row = square->row();
    const int col = square->col();
    auto makeConnection = [this, row, col, square](auto signal, int role) {
        connect(square, signal, this, [this, row, col, role]() {
            QModelIndex idx = createIndex(row * 8 + col, 0);
            emit dataChanged(idx, idx, {role});
        });
    };

    makeConnection(&Square::pieceChanged, PieceRole);
    makeConnection(&Square::legalDestinationChanged, IsLegalDestinationRole);
    makeConnection(&Square::highlightChanged, HighlightRole);
    makeConnection(&Square::isSelectedChanged, IsSelectedRole);
}

QVariant Board::data(const QModelIndex &index, int role) const noexcept
{
    if (!index.isValid() || index.row() < 0 || index.row() >= 64)
        return QVariant();

    const Square *sq = m_squares[index.row() / 8][index.row() % 8];

    switch (role) {
        case RowRole: return sq->row();
        case ColRole: return sq->col();
        case PieceRole: return sq->piece();
        case IsLegalDestinationRole: return sq->isLegalDestination();
        case HighlightRole: return sq->highlight();
        case IsSelectedRole: return sq->isSelected();
    }

    return QVariant();
}

QHash<int, QByteArray> Board::roleNames() const noexcept
{
    return {
        { RowRole, "row" },
        { ColRole, "col" },
        { PieceRole, "piece" },
        { IsLegalDestinationRole, "isLegalDestination" },
        { HighlightRole, "highlight" },
        { IsSelectedRole, "isSelected" }
    };
}
