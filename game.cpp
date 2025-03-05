#include "game.h"
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
    m_isGameOver = false;
    // Additional setup if needed
}

void Game::endGame() {
    m_isGameOver = true;
    // Display a message or handle end game scenarios
}

void Game::makeMove(const QString& move) {
    // Assuming move is in algebraic notation (e.g., "a2 a4")
    QString from = move.mid(0, 2); // Extract "a2"
    QString to = move.mid(3, 2);   // Extract "a4"

    if (isValidMove(from, to)) {
        m_chessboard.movePiece(from, to);
        switchCurrentPlayer();
        // Check for game over conditions (checkmate, stalemate) here
        // checkForCheckmate();
        // checkForStalemate();
    } else {
        // Display an error message to the user (e.g., "Invalid move")
        QMessageBox::warning(nullptr, "Invalid Move", "That is not a valid move.");
    }
}

bool Game::isGameOver() const {
    return m_isGameOver;
}

void Game::initializeGame() {
    m_chessboard.initializeBoard();
    m_currentPlayer = &m_playerWhite; // White player starts
    m_isGameOver = false;
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

void Game::checkForCheckmate() {
    // Implement checkmate detection logic
}

void Game::checkForStalemate() {
    // Implement stalemate detection logic
}

bool Game::isValidMove(const QString& from, const QString& to) const {
    Figure* figure = m_chessboard.getFigureAt(from);
    if (figure == nullptr) {
        return false; // No figure at the starting position
    }

    QVector<QString> availableMoves = figure->availableMoves(from);
    return availableMoves.contains(to); // Check if the move is in the available moves
}

QVector<QString> Game::getAvailableMovesForFigure(const QString& position) const {
    Figure* figure = m_chessboard.getFigureAt(position);
    if (figure == nullptr) {
        return QVector<QString>(); // Return an empty vector if there is no figure
    }
    return figure->availableMoves(position);
}

Figure* Game::getFigureAt(const QString& position) const {
    return m_chessboard.getFigureAt(position);
}
