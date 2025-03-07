#ifndef FIGURE_H
#define FIGURE_H

#include <QString>
#include <QVector>
#include <QPoint>

class Chessboard;
class Figure;

enum class Color {
    WHITE = 0,
    BLACK
};

class Figure {
public:
    Figure(Color color);
    virtual ~Figure() = default;

    Color getColor() const;
    virtual QVector<QString> availableMoves(const QString& position, const Chessboard& board) const = 0;
    virtual QString getTypeString() const = 0;

protected:
    Color m_color;
    QVector<QString> iterateDirections(QVector<QPoint>& directions, int range, const QString& position, const Chessboard& board) const;
};

#endif // FIGURE_H
