#include "pawn.h"
#include "chessboard.h"

#include <QPoint>

Pawn::Pawn(Color color) : Figure(color) {}

QVector<QString> Pawn::availableMoves(const QString& position, const Chessboard& board) const {
    QVector<QString> moves;

    // Get the file and rank from the position
    char file = position[0].toLatin1();
    int rank = position[1].digitValue();

    int direction = getDirection(); // Get the pawn's moving direction

    // 1. One square forward
    char newFile = file;
    int newRank = rank + direction;
    if (newRank >= 1 && newRank <= 8) {
        moves.push_back(QString(QChar(newFile)) + QString::number(newRank));
        // TODO: Add a check here to see if the square is occupied. If it is, the pawn cannot move there.
    }


    // 2. Two squares forward (only on the starting rank)
    if ((m_color == Color::WHITE && rank == 2) || (m_color == Color::BLACK && rank == 7)) {
        newFile = file;
        newRank = rank + 2 * direction;
        if (newRank >= 1 && newRank <= 8) {
             moves.push_back(QString(QChar(newFile)) + QString::number(newRank));
            // TODO: Add a check here to see if both squares in front are unoccupied.
        }
    }

    // 3. Diagonal capture (one square diagonally)
    QVector<QPoint> captureOffsets = {
        {-1, direction}, {1, direction}
    };
    for (const auto& offset : captureOffsets) {
        newFile = file + offset.x();
        newRank = rank + offset.y();
        if (newFile >= 'a' && newFile <= 'h' && newRank >= 1 && newRank <= 8) {
            moves.push_back(QString(QChar(newFile)) + QString::number(newRank));
            // TODO: Add a check here to see if there is an enemy piece on the diagonal square.
        }
    }

    // TODO: Add en passant and pawn promotion logic here

    return moves;
}

QString Pawn::getTypeString() const {
    return "pawn";
}

int Pawn::getDirection() const {
    return m_color == Color::WHITE ? 1 : -1;
}
