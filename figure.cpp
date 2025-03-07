#include "figure.h"
#include "chessboard.h"

#include <QPoint>

Figure::Figure(Color color) : m_color(color) {}

Color Figure::getColor() const {
    return m_color;
}

QVector<QString> Figure::iterateDirections(QVector<QPoint>& directions, int range, const QString& position, const Chessboard& board) const {
    QVector<QString> moves;

    // Get the file and rank from the position
    char file = position[0].toLatin1();
    int rank = position[1].digitValue();


    // Check each direction
    for (const auto& dir : directions) {
        // Start from the current position and move in the current direction
        for (int step = 1; step <= range; ++step) { // Rook can move max 7 steps
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
