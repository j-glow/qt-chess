#ifndef KING_H
#define KING_H

#include "figure.h"
#include <QVector>
#include <QString>

class King : public Figure {
public:
    King(Color color);
    ~King() override = default;

    QVector<QString> availableMoves(const QString& position, const Chessboard& board) const override;
    QString getTypeString() const override;
};

#endif // KING_H
