#ifndef FIGURE_H
#define FIGURE_H

#include <QString>
#include <QVector>

enum class Color {
    WHITE = 0,
    BLACK
};

class Figure {
public:
    Figure(Color color);
    virtual ~Figure() = default;

    Color getColor() const;
    virtual QVector<QString> availableMoves(const QString& position) const = 0;
    virtual QString getTypeString() const = 0;

protected:
    Color m_color;
};

#endif // FIGURE_H
