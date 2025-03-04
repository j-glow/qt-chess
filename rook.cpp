#include "rook.h"

#include <QPoint>

Rook::Rook(Color color) : Figure(color) {}

QVector<QString> Rook::availableMoves(const QString& position) const {
    QVector<QString> moves;

    // Get the file and rank from the position
    char file = position[0].toLatin1();
    int rank = position[1].digitValue();

    // Define directions: 4 orthogonal directions
    QVector<QPoint> directions = {
        {-1, 0}, {1, 0},
        {0, -1}, {0, 1}
    };

    // Check each direction
    for (const auto& dir : directions) {
        // Start from the current position and move in the current direction
        for (int step = 1; step <= 7; ++step) { // Rook can move max 7 steps
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

QString Rook::getTypeString() const {
    return "rook";
}
