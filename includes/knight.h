#ifndef KNIGHT_H
#define KNIGHT_H

#include "Figure.h"
#include <QVector>
#include <QString>

class Knight : public Figure {
public:
    Knight(Color color);
    ~Knight() override = default;

    QVector<QString> availableMoves(const QString& position, const Chessboard& board) const override;
    QString getTypeString() const override;
};

#endif // KNIGHT_H
