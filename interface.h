#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include "game.h" // Assuming you have a Game class

QT_BEGIN_NAMESPACE
namespace Ui {
class Interface;
}
QT_END_NAMESPACE

class Interface : public QMainWindow
{
    Q_OBJECT

public:
    Interface(QWidget *parent = nullptr);
    ~Interface();

private slots:
    void squareClicked(); // Handles square click events
    void newGameClicked(); // Handles new game button click
    void quitGameClicked(); // Handles quit game button click

private:
    Ui::Interface *ui; // Pointer to the UI class (if using Qt Designer)
    QGridLayout *m_chessboardLayout; // Layout for the chessboard
    QLabel *m_squares[8][8]; // 2D array of labels representing the chessboard squares
    Game m_game; // Game logic object
    QPushButton *m_newGameButton; // Button to start a new game
    QPushButton *m_quitGameButton; // Button to quit the game

    void initializeChessboard(); // Initializes the chessboard grid
    void updateChessboard(); // Updates the chessboard display based on the game state
    void clearSelection(); // Clears any square selections
    void highlightPossibleMoves(const QString& position); // Highlights possible moves for a selected piece
    QString getPositionFromSquare(QLabel *square) const; // Gets the chessboard position (e.g., "a1") from a square
};
#endif // INTERFACE_H
