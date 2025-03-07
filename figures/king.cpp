#include "king.h"
#include "chessboard.h"

#include <QPoint>

King::King(Color color) : Figure(color) {}

QVector<QString> King::availableMoves(const QString& position, const Chessboard& board) const {
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

        // Stop if we go out of bounds
        if (newFile < 'a' || newFile > 'h' || newRank < 1 || newRank > 8) {
            continue;
        }

        QString new_position = QString(QChar(newFile)) + QString::number(newRank);            
        Figure* dest = board.getFigureAt(new_position);

        if (!dest)
            moves.push_back(new_position);
        else if(dest->getColor() != this->getColor()){
            // If color of the figure is different, stop iterating but still add move
            moves.push_back(new_position);
            break;
        }
        else //Color is the same, so we can't "step" on the figure 
            break;
    }

    return moves;
}

QString King::getTypeString() const {
    return "king";
}
