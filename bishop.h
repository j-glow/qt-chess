#ifndef BISHOP_H
#define BISHOP_H

#include "figure.h"
#include <QVector>
#include <QString>

class Bishop : public Figure {
public:
    Bishop(Color color);
    ~Bishop() override = default;

    QVector<QString> availableMoves(const QString& position) const override;
    QString getTypeString() const override;
};

#endif // BISHOP_H
