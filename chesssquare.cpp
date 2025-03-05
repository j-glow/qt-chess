#include "ChessSquare.h"

ChessSquare::ChessSquare(QWidget *parent) : QLabel(parent) {}

void ChessSquare::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event); // Avoid unused variable warning

    // Emit the custom signal when the square is clicked
    emit squareClicked(this);
}
