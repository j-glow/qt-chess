#include "chessboard.h"
#include "king.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "pawn.h"
#include <qdebug.h>

Chessboard::Chessboard() {
    // Initialize the board as an 8x8 QVector of Figure pointers
    m_board.resize(8, QVector<Figure *>(8, nullptr));
    initializeBoard(); // Initialize the board in the default constructor
}

Chessboard::Chessboard(const Chessboard& other) {
    // Deep copy the board
    m_board.resize(8);
    for (int row = 0; row < 8; ++row) {
        m_board[row].resize(8);
        for (int col = 0; col < 8; ++col) {
            if (other.m_board[row][col]) {
                // Create a new Figure of the correct type and copy the color
                Color figureColor = other.m_board[row][col]->getColor();
                QString figureType = other.m_board[row][col]->getTypeString();
                if (figureType == "pawn") {
                    m_board[row][col] = new Pawn(figureColor);
                } else if (figureType == "rook") {
                    m_board[row][col] = new Rook(figureColor);
                } else if (figureType == "knight") {
                    m_board[row][col] = new Knight(figureColor);
                } else if (figureType == "bishop") {
                    m_board[row][col] = new Bishop(figureColor);
                } else if (figureType == "queen") {
                    m_board[row][col] = new Queen(figureColor);
                } else if (figureType == "king") {
                    m_board[row][col] = new King(figureColor);
                } else {
                    m_board[row][col] = nullptr;
                }
            } else {
                m_board[row][col] = nullptr;
            }
        }
    }
}

Chessboard& Chessboard::operator=(const Chessboard& other) {
    // Check for self-assignment
    if (this == &other) {
        return *this;
    }

    // 1. Clear existing resources (the old board)
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            delete m_board[row][col];
        }
    }
    m_board.clear();

    // 2. Copy the data (deep copy)
   m_board.resize(8);
    for (int row = 0; row < 8; ++row) {
        m_board[row].resize(8);
        for (int col = 0; col < 8; ++col) {
            if (other.m_board[row][col]) {
                // Create a new Figure of the correct type and copy the color
                Color figureColor = other.m_board[row][col]->getColor();
                QString figureType = other.m_board[row][col]->getTypeString();
                if (figureType == "pawn") {
                    m_board[row][col] = new Pawn(figureColor);
                } else if (figureType == "rook") {
                    m_board[row][col] = new Rook(figureColor);
                } else if (figureType == "knight") {
                    m_board[row][col] = new Knight(figureColor);
                } else if (figureType == "bishop") {
                    m_board[row][col] = new Bishop(figureColor);
                } else if (figureType == "queen") {
                    m_board[row][col] = new Queen(figureColor);
                } else if (figureType == "king") {
                    m_board[row][col] = new King(figureColor);
                } else {
                    m_board[row][col] = nullptr; // Or handle this case appropriately
                }
            } else {
                m_board[row][col] = nullptr;
            }
        }
    }

    // 3. Return a reference to the object (for chaining)
    return *this;
}

Chessboard::~Chessboard() {
    // Clear the board and delete figures
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            delete m_board[row][col];
        }
        m_board[row].clear();
    }
    m_board.clear();
}

void Chessboard::initializeBoard() {
    // Clear the board first (in case it's not the first time this is called)
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            delete m_board[row][col];
        }
        m_board[row].clear();
    }
    m_board.clear();
    m_board.resize(8);
    for (int i = 0; i < 8; i++)
        m_board[i].resize(8);

    // Initialize the board with figures
    // Black pieces
    m_board[0][0] = new Rook(Color::BLACK);
    m_board[0][1] = new Knight(Color::BLACK);
    m_board[0][2] = new Bishop(Color::BLACK);
    m_board[0][3] = new Queen(Color::BLACK);
    m_board[0][4] = new King(Color::BLACK);
    m_board[0][5] = new Bishop(Color::BLACK);
    m_board[0][6] = new Knight(Color::BLACK);
    m_board[0][7] = new Rook(Color::BLACK);
    for (int i = 0; i < 8; i++) {
        m_board[1][i] = new Pawn(Color::BLACK);
    }

    // White pieces
    m_board[7][0] = new Rook(Color::WHITE);
    m_board[7][1] = new Knight(Color::WHITE);
    m_board[7][2] = new Bishop(Color::WHITE);
    m_board[7][3] = new Queen(Color::WHITE);
    m_board[7][4] = new King(Color::WHITE);
    m_board[7][5] = new Bishop(Color::WHITE);
    m_board[7][6] = new Knight(Color::WHITE);
    m_board[7][7] = new Rook(Color::WHITE);
    for (int i = 0; i < 8; i++) {
        m_board[6][i] = new Pawn(Color::WHITE);
    }

    // Empty squares
    for (int row = 2; row < 6; ++row) {
        for (int col = 0; col < 8; ++col) {
            m_board[row][col] = nullptr;
        }
    }
}

Figure* Chessboard::getFigureAt(const QString& position) const {
    int row = 8 - position[1].digitValue();
    int col = position[0].toLatin1() - 'a';

    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        return m_board[row][col];
    }
    return nullptr;
}

void Chessboard::setFigureAt(const QString& position, Figure* figure) {
    int row = 8 - position[1].digitValue();
    int col = position[0].toLatin1() - 'a';
    
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        m_board[row][col] = figure;
    }
}

void Chessboard::movePiece(const QString& from, const QString& to) {
    Figure* dest = getFigureAt(to);
    if (dest)
        delete dest;

    Figure* figure = getFigureAt(from);
    setFigureAt(from, nullptr);
    setFigureAt(to, figure);
}
