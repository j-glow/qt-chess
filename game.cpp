#include "game.h"
#include "interface.h"

#include <QMessageBox>

Game::Game() :
    m_playerWhite(Color::WHITE),
    m_playerBlack(Color::BLACK),
    m_currentPlayer(&m_playerWhite) // White player starts
{
    startGame();
}

Game::~Game() {
}

void Game::startGame() {
    m_chessboard.initializeBoard();
    m_currentPlayer = &m_playerWhite; // White player starts
    m_whiteCastlingOptions = CastlingOptions::BOTH;
    m_blackCastlingOptions = CastlingOptions::BOTH;
}

void Game::endGame() {
    Interface::endGamePopup(m_winner); // Use the static method
}

void Game::makeMove(const QString& move) {
    // Assuming move is in algebraic notation (e.g., "a2 a4")
    QString from = move.mid(0, 2); // Extract "a2"
    QString to = move.mid(3, 2);   // Extract "a4"

    if (isValidMove(from, to)) {
        // If king moved, change castling options
        if (m_chessboard.getFigureAt(from)->getTypeString() == "king"){
            if (from == QString("e1"))
                m_whiteCastlingOptions = CastlingOptions::NONE;
            if (from == QString("e8"))
                m_blackCastlingOptions = CastlingOptions::NONE;
        } // else if rook moved, change castling options
        else if (m_chessboard.getFigureAt(from)->getTypeString() == "rook"){
            if (from == QString("a1"))
                m_whiteCastlingOptions = static_cast<CastlingOptions>(m_whiteCastlingOptions & CastlingOptions::KINGSIDE);
            if (from == QString("h1"))
                m_whiteCastlingOptions = static_cast<CastlingOptions>(m_whiteCastlingOptions & CastlingOptions::QUEENSIDE);
            if (from == QString("a8"))
                m_blackCastlingOptions = static_cast<CastlingOptions>(m_blackCastlingOptions & CastlingOptions::KINGSIDE);
            if (from == QString("h8"))
                m_blackCastlingOptions = static_cast<CastlingOptions>(m_blackCastlingOptions & CastlingOptions::QUEENSIDE);
        }

        // If it's castling move the rook as well
        if(m_chessboard.getFigureAt(from)->getTypeString() == "king"){
            int row = (m_currentPlayer->getColor() == Color::WHITE) ? 1 : 8; // Determine the correct row
            if (to == QString("g") + QString::number(row))
                m_chessboard.movePiece(QString("h") + QString::number(row), QString("f") + QString::number(row));
            if (to == QString("c") + QString::number(row))
                m_chessboard.movePiece(QString("a") + QString::number(row), QString("d") + QString::number(row));
        }

        m_chessboard.movePiece(from, to);

        // Pawn promotion
        if (m_chessboard.getFigureAt(to)->getTypeString()=="pawn") {
            if(m_currentPlayer->getColor() == Color::WHITE && to.endsWith("8") ||
               m_currentPlayer->getColor() == Color::WHITE && to.endsWith("1"))
                promotePawn(to);
        }

        // Switch player and check if game can continue
        switchCurrentPlayer();
        if(checkForGameOver())
            endGame();
    }
}

void Game::switchCurrentPlayer() {
    if (m_currentPlayer == &m_playerWhite) {
        m_currentPlayer = &m_playerBlack;
    } else {
        m_currentPlayer = &m_playerWhite;
    }
}

bool Game::checkForGameOver() {
    if (!canPlayerMakeAnyLegalMove(m_currentPlayer)) {
        if (isPlayerInCheck(m_chessboard, m_currentPlayer))
            m_winner = (m_currentPlayer->getColor() == Color::WHITE) ? &m_playerBlack : &m_playerWhite;
        return true;
    }
    return false;
}

bool Game::canPlayerMakeAnyLegalMove(Player* player) const {
    Chessboard temp_board = m_chessboard;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QString position = QString(QChar('a' + col)) + QString::number(8 - row);
            Figure* figure = temp_board.getFigureAt(position);
            if (figure && figure->getColor() == player->getColor()) {
                QVector<QString> availableMoves = getAvailableMovesForFigure(position);
                for (const QString& move : availableMoves) {
                    // 1. Create a temporary copy of the dest figure
                    Figure* destFigure = temp_board.getFigureAt(move);
                    temp_board.setFigureAt(move, nullptr);

                    // 2. Temporarily make the move on the copy
                    temp_board.movePiece(position, move);

                    // 3. Check if the move leaves the player's king in check
                    if (!isPlayerInCheck(temp_board, player)) {
                        temp_board.movePiece(move, position);
                        temp_board.setFigureAt(move, destFigure);
                        return true; // Found a legal move
                    }

                    // 4. Undo the move
                    temp_board.movePiece(move, position);
                    temp_board.setFigureAt(move, destFigure);
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

    QVector<QString> availableMoves = getAvailableMovesForFigure(from);
    if (!availableMoves.contains(to))
        return false; // The move is not in the figure's available moves

    // Temporarily make the move
    Chessboard tempBoard = m_chessboard; // Create a temporary board
    tempBoard.movePiece(from, to);

    // 2. Check if the move leaves the current player's king in check
    if (isPlayerInCheck(tempBoard, m_currentPlayer))
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

    // Get default moves for a figure
    QVector<QString> availableMoves = figure->availableMoves(position, m_chessboard);

    // Add castling to possible moves
    if (figure->getTypeString() == "king") {
        int row = (m_currentPlayer->getColor() == Color::WHITE) ? 1 : 8; // Determine the correct row
        CastlingOptions options = isCastlingAllowed(m_currentPlayer->getColor());

        if (position == QString("e") + QString::number(row)){
            if(options & CastlingOptions::KINGSIDE)
                availableMoves.push_back(QString("g") + QString::number(row));
            if(options & CastlingOptions::QUEENSIDE)
                availableMoves.push_back(QString("c") + QString::number(row));
        }
    }

    // TODO: Add en passant

    return availableMoves;
}

Color Game::currentPlayerColor() const {
    return m_currentPlayer->getColor();
}


Figure* Game::getFigureAt(const QString& position) const {
    return m_chessboard.getFigureAt(position);
}

bool Game::isPlayerInCheck(const Chessboard& chessboard, const Player* player) const {
    QString kingPosition = getKingPosition(chessboard, player);
    return isSquareAttacked(chessboard, kingPosition, player);
}

bool Game::isSquareAttacked(const Chessboard& board, const QString& position, const Player* player) const {
    const Player* opponent = (player == &m_playerWhite) ? &m_playerBlack : &m_playerWhite;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QString position = QString(QChar('a' + col)) + QString::number(8 - row);
            Figure* figure = board.getFigureAt(position);
            if (figure && figure->getColor() == opponent->getColor()) {
                QVector<QString> availableMoves = figure->availableMoves(position, board);
                if (availableMoves.contains(position)) {
                    return true;
                }
            }
        }
    }
    return false;
}

QString Game::getKingPosition(const Chessboard& chessboard, const Player* player) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QString position = QString(QChar('a' + col)) + QString::number(8 - row);
            Figure* figure = chessboard.getFigureAt(position);
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

CastlingOptions Game::isCastlingAllowed(Color color) const {
    CastlingOptions options = CastlingOptions::NONE;
    int row = (color == Color::WHITE) ? 1 : 8; // Determine the correct row
    const Player* opponent = (color == Color::WHITE) ? &m_playerBlack : &m_playerWhite;

    // Kingside check
    if ((color == Color::WHITE && (m_whiteCastlingOptions & CastlingOptions::KINGSIDE)) ||
        (color == Color::BLACK && (m_blackCastlingOptions & CastlingOptions::KINGSIDE))) {
        // Check if squares are empty
        if (m_chessboard.getFigureAt(QString("f") + QString::number(row)) == nullptr &&
            m_chessboard.getFigureAt(QString("g") + QString::number(row)) == nullptr) {
            // Check if king is in check or passes through check
            QString kingStartPos = QString("e") + QString::number(row);
            QString kingMidPos = QString("f") + QString::number(row);
            QString kingEndPos = QString("g") + QString::number(row);

            if (!isSquareAttacked(m_chessboard, kingStartPos, opponent) &&
                !isSquareAttacked(m_chessboard, kingMidPos, opponent) &&
                !isSquareAttacked(m_chessboard, kingEndPos, opponent)) {
                options = static_cast<CastlingOptions>(options | CastlingOptions::KINGSIDE);
            }
        }
    }

    // Queenside check
    if ((color == Color::WHITE && (m_whiteCastlingOptions & CastlingOptions::QUEENSIDE)) ||
        (color == Color::BLACK && (m_blackCastlingOptions & CastlingOptions::QUEENSIDE))) {
        // Check if squares are empty
        if (m_chessboard.getFigureAt(QString("b") + QString::number(row)) == nullptr &&
            m_chessboard.getFigureAt(QString("c") + QString::number(row)) == nullptr &&
            m_chessboard.getFigureAt(QString("d") + QString::number(row)) == nullptr) {
            // Check if king is in check or passes through check
            QString kingStartPos = QString("e") + QString::number(row);
            QString kingMidPos = QString("d") + QString::number(row);
            QString kingEndPos = QString("c") + QString::number(row);

            if (!isSquareAttacked(m_chessboard, kingStartPos, opponent) &&
                !isSquareAttacked(m_chessboard, kingMidPos, opponent) &&
                !isSquareAttacked(m_chessboard, kingEndPos, opponent)) {
                options = static_cast<CastlingOptions>(options | CastlingOptions::QUEENSIDE);
            }
        }
    }

    return options;
}
