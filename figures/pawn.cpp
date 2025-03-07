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
    QString new_position = QString(QChar(newFile)) + QString::number(newRank);
    if (newRank >= 1 && newRank <= 8 && !board.getFigureAt(new_position)) {
        moves.push_back(new_position);
    }

    // 2. Two squares forward (only on the starting rank)
    // If moves is empty, it means that the first square is occupied, hence can't move 2
    if (moves.size() && (m_color == Color::WHITE && rank == 2) || (m_color == Color::BLACK && rank == 7)) {
        newFile = file;
        newRank = rank + 2 * direction;
        QString new_position = QString(QChar(newFile)) + QString::number(newRank);
        if (newRank >= 1 && newRank <= 8 && !board.getFigureAt(new_position)) {
            moves.push_back(new_position);
        }
    }

    // 3. Diagonal capture (one square diagonally)
    QVector<QPoint> captureOffsets = {
        {-1, direction}, {1, direction}
    };
    for (const auto& offset : captureOffsets) {
        newFile = file + offset.x();
        newRank = rank + offset.y();
        QString new_position = QString(QChar(newFile)) + QString::number(newRank);
        if (newFile >= 'a' && newFile <= 'h' && newRank >= 1 && newRank <= 8) {
            if (board.getFigureAt(new_position))
                moves.push_back(new_position);
        }
    }

    // En passant and promotion logic will be added to game engine, as it reuires knowledge
    // of the last move and changing of the figure type (out of scope for pawn class)

    return moves;
}

QString Pawn::getTypeString() const {
    return "pawn";
}

int Pawn::getDirection() const {
    return m_color == Color::WHITE ? 1 : -1;
}
