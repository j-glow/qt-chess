#include "rook.h"
#include "chessboard.h"

#include <QPoint>

Rook::Rook(Color color) : Figure(color) {}

QVector<QString> Rook::availableMoves(const QString& position, const Chessboard& board) const {
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

            // Stop if we go out of bounds
            if (newFile < 'a' || newFile > 'h' || newRank < 1 || newRank > 8) {
                break;
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
    }

    return moves;
}

QString Rook::getTypeString() const {
    return "rook";
}
