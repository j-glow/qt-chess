#include "queen.h"
#include "chessboard.h"

#include <QPoint>

Queen::Queen(Color color) : Figure(color) {}

QVector<QString> Queen::availableMoves(const QString& position, const Chessboard& board) const {
    // Define directions: 8 directions (horizontal, vertical, and diagonal)
    QVector<QPoint> directions = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},          {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };

    return iterateDirections(directions, /*range=*/ 7, position, board);
}

QString Queen::getTypeString() const {
    return "queen";
}
