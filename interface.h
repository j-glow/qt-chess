#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include "game.h"
#include "chesssquare.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Interface;
}
QT_END_NAMESPACE

class Interface : public QMainWindow {
    Q_OBJECT

public:
    Interface(QWidget *parent = nullptr);
    ~Interface();

private slots:
    void squareClicked(ChessSquare *square);
    void newGameClicked();
    void quitGameClicked();

private:
    Ui::Interface *ui;
    QGridLayout *m_chessboardLayout;
    ChessSquare *m_squares[8][8];
    Game m_game;
    QPushButton *m_newGameButton;
    QPushButton *m_quitGameButton;

    void initializeChessboard();
    void updateChessboard();
    void clearSelection();
    void highlightPossibleMoves(const QString &position);
    QString getPositionFromSquare(ChessSquare *square) const;
};
#endif // INTERFACE_H
