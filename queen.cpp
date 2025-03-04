#include "queen.h"

#include <QPoint>

Queen::Queen(Color color) : Figure(color) {}

QVector<QString> Queen::availableMoves(const QString& position) const {
    QVector<QString> moves;

    // Get the file and rank from the position
    char file = position[0].toLatin1();
    int rank = position[1].digitValue();

    // Define directions: 8 directions (horizontal, vertical, and diagonal)
    QVector<QPoint> directions = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},          {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };

    // Check each direction
    for (const auto& dir : directions) {
        // Start from the current position and move in the current direction
        for (int step = 1; step <= 7; ++step) { // Queen can move max 7 steps
            char newFile = file + step * dir.x();
            int newRank = rank + step * dir.y();

            // Check if the new position is within the board bounds
            if (newFile >= 'a' && newFile <= 'h' && newRank >= 1 && newRank <= 8) {
                moves.push_back(QString(QChar(newFile)) + QString::number(newRank));
            } else {
                // Stop if we go out of bounds
                break;
            }

            // TODO: Add logic to check if the path is blocked by another piece.
            // If the path is blocked, you should also break here.
        }
    }

    return moves;
}

QString Queen::getTypeString() const {
    return "queen";
}
