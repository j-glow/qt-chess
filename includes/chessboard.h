#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QVector>
#include <QString>
#include "figure.h"

class Chessboard {
public:
    Chessboard();
    ~Chessboard();

    void initializeBoard();
    Figure* getFigureAt(const QString& position) const;
    void setFigureAt(const QString& position, Figure* figure);
    void movePiece(const QString& from, const QString& to);

private:
    QVector<QVector<Figure*>> m_board; // 2D QVector to represent the board
};

#endif // CHESSBOARD_H
