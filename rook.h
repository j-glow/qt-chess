#ifndef ROOK_H
#define ROOK_H

#include "figure.h"
#include <QVector>
#include <QString>

class Rook : public Figure {
public:
    Rook(Color color);
    ~Rook() override = default;

    QVector<QString> availableMoves(const QString& position) const override;
    QString getTypeString() const override;
};

#endif // ROOK_H
