#ifndef PAWN_H
#define PAWN_H

#include "figure.h"
#include <QVector>
#include <QString>

class Pawn : public Figure {
public:
    Pawn(Color color);
    ~Pawn() override = default;

    QVector<QString> availableMoves(const QString& position) const override;
    QString getTypeString() const override;

private:
    int getDirection() const; // Helper function to get the pawn's direction
};

#endif // PAWN_H
