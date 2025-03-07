#include "king.h"

#include <QPoint>

King::King(Color color) : Figure(color) {}

QVector<QString> King::availableMoves(const QString& position) const {
    QVector<QString> moves;

    // Get the file and rank from the position
    char file = position[0].toLatin1();
    int rank = position[1].digitValue();

    // Define possible moves (relative to the current position)
    QVector<QPoint> moveOffsets = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},          {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };

    // Iterate through possible moves
    for (const auto& offset : moveOffsets) {
        // Calculate the new file and rank
        char newFile = file + offset.x();
        int newRank = rank + offset.y();

        // Check if the new position is within the board bounds
        if (newFile >= 'a' && newFile <= 'h' && newRank >= 1 && newRank <= 8) {
            moves.push_back(QString(QChar(newFile)) + QString::number(newRank));
        }
    }

    return moves;
}

QString King::getTypeString() const {
    return "king";
}
