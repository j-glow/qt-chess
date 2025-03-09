#ifndef GAME_H
#define GAME_H

#include "chessboard.h"
#include "player.h"
#include "figure.h"
#include <QString>

enum CastlingOptions {
    NONE = 0b00,
    KINGSIDE = 0b01,
    QUEENSIDE = 0b10,
    BOTH = KINGSIDE | QUEENSIDE
};

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
    Player* m_winner;
    CastlingOptions m_whiteCastlingOptions = CastlingOptions::BOTH;
    CastlingOptions m_blackCastlingOptions = CastlingOptions::BOTH;

    void switchCurrentPlayer();
    bool checkForGameOver();
    bool canPlayerMakeAnyLegalMove(Player* player) const;
    bool isPlayerInCheck(const Chessboard& chessboard, const Player* player) const;
    QString getKingPosition(const Chessboard& chessboard, const Player* player) const;
    void promotePawn(const QString& position);
    enum CastlingOptions isCastlingAllowed(Color color) const;
    bool isSquareAttacked(const Chessboard& board, const QString& position, const Player* player) const;
};

#endif // GAME_H
