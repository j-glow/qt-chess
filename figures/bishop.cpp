#include "bishop.h"
#include "chessboard.h"

#include <QPoint>

Bishop::Bishop(Color color) : Figure(color) {}

QVector<QString> Bishop::availableMoves(const QString& position, const Chessboard& board) const {
    // Define directions: 4 diagonal directions
    QVector<QPoint> directions = {
        {-1, -1}, {-1, 1},
        {1, -1}, {1, 1}
    };

    return iterateDirections(directions, /*range=*/ 7, position, board);
}

QString Bishop::getTypeString() const {
    return "bishop";
}
