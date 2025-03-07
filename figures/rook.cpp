#include "rook.h"
#include "chessboard.h"

#include <QPoint>

Rook::Rook(Color color) : Figure(color) {}

QVector<QString> Rook::availableMoves(const QString& position, const Chessboard& board) const {
    // Define directions: 4 orthogonal directions
    QVector<QPoint> directions = {
        {-1, 0}, {1, 0},
        {0, -1}, {0, 1}
    };

    return iterateDirections(directions, /*range=*/ 7, position, board);
}

QString Rook::getTypeString() const {
    return "rook";
}
