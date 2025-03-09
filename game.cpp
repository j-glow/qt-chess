#include "game.h"
#include "interface.h"

#include <QMessageBox>

Game::Game() :
    m_playerWhite(Color::WHITE),
    m_playerBlack(Color::BLACK),
    m_currentPlayer(&m_playerWhite), // White player starts
    m_isGameOver(false)
{
    initializeGame();
}

Game::~Game() {
}

void Game::startGame() {
    m_chessboard.initializeBoard();
    m_currentPlayer = &m_playerWhite; // White player starts
}

#include <QDebug>
void Game::endGame() {
    qDebug() << "mate";
}

void Game::makeMove(const QString& move) {
    // Assuming move is in algebraic notation (e.g., "a2 a4")
    QString from = move.mid(0, 2); // Extract "a2"
    QString to = move.mid(3, 2);   // Extract "a4"

    if (isValidMove(from, to)) {
        m_chessboard.movePiece(from, to);

        if (m_chessboard.getFigureAt(to)->getTypeString()=="pawn") {
            if(m_currentPlayer->getColor() == Color::WHITE && to.endsWith("8") ||
               m_currentPlayer->getColor() == Color::WHITE && to.endsWith("1"))
                promotePawn(to);
        }

        switchCurrentPlayer();
        if(checkForGameOver())
            endGame();
    }
}

void Game::initializeGame() {
    m_chessboard.initializeBoard();
    m_currentPlayer = &m_playerWhite; // White player starts
}

void Game::switchCurrentPlayer() {
    if (m_currentPlayer == &m_playerWhite) {
        m_currentPlayer = &m_playerBlack;
    } else {
        m_currentPlayer = &m_playerWhite;
    }
}

void Game::handleSpecialMoves(const QString& move) {
    // Implement logic for castling, en passant, pawn promotion
    // This will require more complex parsing of the move string
}

bool Game::checkForGameOver() {
    if (!canPlayerMakeAnyLegalMove(m_currentPlayer)) {
        if (isPlayerInCheck(m_chessboard, m_currentPlayer))
            m_winner = m_currentPlayer;
        return true;
    }
    return false;
}

bool Game::canPlayerMakeAnyLegalMove(Player* player) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QString position = QString(QChar('a' + col)) + QString::number(8 - row);
            Figure* figure = m_chessboard.getFigureAt(position);
            if (figure && figure->getColor() == player->getColor()) {
                QVector<QString> availableMoves = figure->availableMoves(position, m_chessboard);
                for (const QString& move : availableMoves) {
                    // 1. Create a temporary copy of the chessboard
                    Chessboard tempBoard = m_chessboard;

                    // 2. Temporarily make the move on the copy
                    tempBoard.movePiece(position, move);

                    // 3. Check if the move leaves the player's king in check
                    if (!isPlayerInCheck(tempBoard, player)) {
                        return true; // Found a legal move
                    }
                    // No need to "undo" the move, tempBoard will be destroyed
                }
            }
        }
    }
    return false; // No legal moves found
}

bool Game::isValidMove(const QString& from, const QString& to) const {
    Figure* figure = m_chessboard.getFigureAt(from);
    if (!figure)
        return false; // No figure at the starting position

    QVector<QString> availableMoves = figure->availableMoves(from, m_chessboard);
    if (!availableMoves.contains(to))
        return false; // The move is not in the figure's available moves

    // Temporarily make the move
    Chessboard tempBoard = m_chessboard; // Create a temporary board
    tempBoard.movePiece(from, to);

    // 2. Check if the move leaves the current player's king in check
    const Player* currentPlayer = (m_currentPlayer == &m_playerWhite) ? &m_playerWhite : &m_playerBlack;
    if (isPlayerInCheck(m_chessboard, currentPlayer))
        return false; // The move leaves the king in check, so it's invalid

    // The move is valid (it's in available moves and doesn't cause check)
    return true;
}

QVector<QString> Game::getAvailableMovesForFigure(const QString& position) const {
    Figure* figure = m_chessboard.getFigureAt(position);
    if (figure == nullptr) {
        return QVector<QString>(); // Return an empty vector if there is no figure
    }

    if (figure->getColor() != m_currentPlayer->getColor())
        return QVector<QString>(); // Can't move other color figure

    return figure->availableMoves(position, m_chessboard);
}

Color Game::currentPlayerColor() const {
    return m_currentPlayer->getColor();
}


Figure* Game::getFigureAt(const QString& position) const {
    return m_chessboard.getFigureAt(position);
}

bool Game::isPlayerInCheck(const Chessboard& chessboard, const Player* player) const {
    // 1. Find the player's King
    QString kingPosition = getKingPosition(player);

    // 2. Check if any opponent's piece can attack the King
    const Player* opponent = (player == &m_playerWhite) ? &m_playerBlack : &m_playerWhite;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QString position = QString(QChar('a' + col)) + QString::number(8 - row);
            Figure* figure = chessboard.getFigureAt(position);
            if (figure && figure->getColor() == opponent->getColor()) {
                QVector<QString> availableMoves = figure->availableMoves(position, chessboard);
                if (availableMoves.contains(kingPosition)) {
                    return true; // King is in check
                }
            }
        }
    }
    return false; // King is not in check
}

QString Game::getKingPosition(const Player* player) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QString position = QString(QChar('a' + col)) + QString::number(8 - row);
            Figure* figure = m_chessboard.getFigureAt(position);
            if (figure && figure->getColor() == player->getColor() && figure->getTypeString() == "king") {
                return position;
            }
        }
    }
    return ""; // Should not happen, but handle it anyway
}

void Game::promotePawn(const QString& position) {
    Figure* pawn = m_chessboard.getFigureAt(position);
    if (!pawn || pawn->getTypeString() != "pawn") {
        return; // Or handle this error appropriately
    }

    Color color = pawn->getColor();
    Figure* newFigure = Interface::promoteWindow(color);

    if (newFigure) {
        m_chessboard.setFigureAt(position, newFigure);
        delete pawn; // Remove the pawn
    }
}
