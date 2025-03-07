#include "king.h"
#include "chessboard.h"

#include <QPoint>

King::King(Color color) : Figure(color) {}

QVector<QString> King::availableMoves(const QString& position, const Chessboard& board) const {
    // Define possible moves (relative to the current position)
    QVector<QPoint> directions = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},          {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };

    return iterateDirections(directions, /*range=*/ 1, position, board);
}

QString King::getTypeString() const {
    return "king";
}
