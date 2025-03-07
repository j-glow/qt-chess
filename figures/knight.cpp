#include "knight.h"
#include "chessboard.h"

#include <QPoint>

Knight::Knight(Color color) : Figure(color) {}

QVector<QString> Knight::availableMoves(const QString& position, const Chessboard& board) const {
    QVector<QString> moves;

    // Get the file and rank from the position
    char file = position[0].toLatin1();
    int rank = position[1].digitValue();

    // Define possible moves (L-shape pattern)
    QVector<QPoint> movesOffsets = {
        {-2, -1}, {-2, 1},
        {-1, -2}, {-1, 2},
        {1, -2},  {1, 2},
        {2, -1},  {2, 1}
    };

    // Check each possible move
    for (const auto& offset : movesOffsets) {
        char newFile = file + offset.x();
        int newRank = rank + offset.y();
        QString new_position = QString(QChar(newFile)) + QString::number(newRank);

        // Check if the new position is within the board bounds
        if (newFile >= 'a' && newFile <= 'h' && newRank >= 1 && newRank <= 8) {
            Figure* dest = board.getFigureAt(new_position);
            // Can't jump on own pieces.
            if (!dest || dest->getColor() != this->getColor())
                moves.push_back(new_position);
        }
    }

    return moves;
}

QString Knight::getTypeString() const {
    return "knight";
}
