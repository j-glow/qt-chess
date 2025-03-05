#ifndef CHESSSQUARE_H
#define CHESSSQUARE_H

#include <QLabel>
#include <QMouseEvent>
#include <QString>

class ChessSquare : public QLabel {
    Q_OBJECT

public:
    ChessSquare(QWidget *parent = nullptr);
    ~ChessSquare() override = default;

    QString position() const { return m_position; }
    void setPosition(const QString& position) { m_position = position; }

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void squareClicked(ChessSquare *square);

private:
    QString m_position;
};

#endif // CHESSSQUARE_H
