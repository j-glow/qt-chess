#ifndef QUEEN_H
#define QUEEN_H

#include "figure.h"
#include <QVector>
#include <QString>

class Queen : public Figure {
public:
    Queen(Color color);
    ~Queen() override = default;

    QVector<QString> availableMoves(const QString& position, const Chessboard& board) const override;
    QString getTypeString() const override;
};

#endif // QUEEN_H
