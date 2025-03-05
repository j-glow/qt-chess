#include "chessboard.h"
#include "king.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "pawn.h"

Chessboard::Chessboard() {
    // Initialize the board as an 8x8 QVector of Figure pointers
    m_board.resize(8, QVector<Figure *>(8, nullptr));
}

Chessboard::~Chessboard() {
    // Delete all figures on the board
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (m_board[row][col] != nullptr) {
                delete m_board[row][col];
                m_board[row][col] = nullptr;
            }
        }
    }
}

void Chessboard::initializeBoard() {
    // Create and place white pieces
    m_board[7][0] = new Rook(Color::WHITE);
    m_board[7][1] = new Knight(Color::WHITE);
    m_board[7][2] = new Bishop(Color::WHITE);
    m_board[7][3] = new Queen(Color::WHITE);
    m_board[7][4] = new King(Color::WHITE);
    m_board[7][5] = new Bishop(Color::WHITE);
    m_board[7][6] = new Knight(Color::WHITE);
    m_board[7][7] = new Rook(Color::WHITE);
    for (int i = 0; i < 8; ++i) {
        m_board[6][i] = new Pawn(Color::WHITE);
    }

    // Create and place black pieces
    m_board[0][0] = new Rook(Color::BLACK);
    m_board[0][1] = new Knight(Color::BLACK);
    m_board[0][2] = new Bishop(Color::BLACK);
    m_board[0][3] = new Queen(Color::BLACK);
    m_board[0][4] = new King(Color::BLACK);
    m_board[0][5] = new Bishop(Color::BLACK);
    m_board[0][6] = new Knight(Color::BLACK);
    m_board[0][7] = new Rook(Color::BLACK);
    for (int i = 0; i < 8; ++i) {
        m_board[1][i] = new Pawn(Color::BLACK);
    }
}

Figure* Chessboard::getFigureAt(const QString& position) const {
    int row = 8 - position[1].digitValue();
    int col = position[0].toLatin1() - 'a';

    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        return m_board[row][col];
    }
    return nullptr; // Return nullptr if the position is invalid
}

void Chessboard::setFigureAt(const QString& position, Figure* figure) {
    int row = 8 - position[1].digitValue();
    int col = position[0].toLatin1() - 'a';

    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        m_board[row][col] = figure;
    }
}

void Chessboard::movePiece(const QString& from, const QString& to) {
    Figure* figure = getFigureAt(from);
    if (figure != nullptr) {
        setFigureAt(from, nullptr); // Remove the figure from the source position
        setFigureAt(to, figure);   // Place the figure at the destination position
    }
}


