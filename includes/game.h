#ifndef GAME_H
#define GAME_H

#include "chessboard.h"
#include "player.h"
#include "figure.h"
#include <QString>

class Game {
public:
    Game();
    ~Game();

    void startGame();
    void endGame();
    void makeMove(const QString& move); // Takes move in algebraic notation (e.g., "a2 a4")
    bool isGameOver() const;
    Figure* getFigureAt(const QString& position) const;
    bool isValidMove(const QString& from, const QString& to) const;
    QVector<QString> getAvailableMovesForFigure(const QString& position) const;
    Color currentPlayerColor() const;

private:
    Chessboard m_chessboard;
    Player m_playerWhite;
    Player m_playerBlack;
    Player* m_currentPlayer; // Pointer to the current player
    bool m_isGameOver;

    void initializeGame();
    void switchCurrentPlayer();
    void handleSpecialMoves(const QString& move);
    void checkForCheckmate();
    void checkForStalemate();
    bool isPlayerInCheck(const Player* player) const;
    QString getKingPosition(const Player* player) const;
};

#endif // GAME_H
