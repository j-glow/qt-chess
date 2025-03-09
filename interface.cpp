#include "interface.h"
#include "ui_interface.h"
#include "figure.h"
#include "bishop.h"
#include "knight.h"
#include "rook.h"
#include "queen.h"

#include <QMessageBox>

Interface::Interface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Interface)
{
    ui->setupUi(this);

    // Create main layouts
    QVBoxLayout *mainLayout = new QVBoxLayout; // Main vertical layout
    QHBoxLayout *bottomLayout = new QHBoxLayout;  // Bottom horizontal layout (buttons, etc.)
    QGridLayout *boardLayout = new QGridLayout; // Layout for chessboard and markings

    // 1. Button Layout
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    m_newGameButton = new QPushButton("New Game", this);
    m_quitGameButton = new QPushButton("Quit", this);
    buttonLayout->addWidget(m_newGameButton);
    buttonLayout->addWidget(m_quitGameButton);

    // 2. Chessboard Layout with Markings
    QGridLayout *chessboardAndMarkingsLayout = new QGridLayout; // 3x3 grid

    // Create individual layouts for markings and chessboard
    QGridLayout *topMarkingsLayout = new QGridLayout;
    QGridLayout *bottomMarkingsLayout = new QGridLayout;
    QGridLayout *leftMarkingsLayout = new QGridLayout;
    QGridLayout *rightMarkingsLayout = new QGridLayout;
    QGridLayout *actualChessboardLayout = new QGridLayout; // For the squares

    initializeChessboard(actualChessboardLayout); // Initialize chessboard squares

    // Add column markings (a-h)
    for (int col = 0; col < 8; ++col) {
        QLabel *colLabelTop = new QLabel(QString(QChar('a' + col)));
        colLabelTop->setAlignment(Qt::AlignCenter);
        topMarkingsLayout->addWidget(colLabelTop, 0, col);

        QLabel *colLabelBottom = new QLabel(QString(QChar('a' + col)));
        colLabelBottom->setAlignment(Qt::AlignCenter);
        bottomMarkingsLayout->addWidget(colLabelBottom, 0, col);
    }

    // Add row markings (1-8)
    for (int row = 0; row < 8; ++row) {
        QLabel *rowLabelLeft = new QLabel(QString::number(8 - row));
        rowLabelLeft->setAlignment(Qt::AlignCenter);
        leftMarkingsLayout->addWidget(rowLabelLeft, row, 0);

        QLabel *rowLabelRight = new QLabel(QString::number(8 - row));
        rowLabelRight->setAlignment(Qt::AlignCenter);
        rightMarkingsLayout->addWidget(rowLabelRight, row, 0);
    }

    // Add layouts to chessboardAndMarkingsLayout (3x3 grid)
    chessboardAndMarkingsLayout->addLayout(topMarkingsLayout, 0, 1);    // Top (columns)
    chessboardAndMarkingsLayout->addLayout(leftMarkingsLayout, 1, 0);   // Left (rows)
    chessboardAndMarkingsLayout->addLayout(actualChessboardLayout, 1, 1); // Center (chessboard)
    chessboardAndMarkingsLayout->addLayout(rightMarkingsLayout, 1, 2);  // Right (rows)
    chessboardAndMarkingsLayout->addLayout(bottomMarkingsLayout, 2, 1); // Bottom (columns)

    // Add layouts to the main layout
    bottomLayout->addLayout(buttonLayout);
    boardLayout->addLayout(chessboardAndMarkingsLayout, 0, 0);

    mainLayout->addLayout(boardLayout);
    mainLayout->addLayout(bottomLayout);

    // Central Widget Setup
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Connect signals and slots
    connect(m_newGameButton, &QPushButton::clicked, this, &Interface::newGameClicked);
    connect(m_quitGameButton, &QPushButton::clicked, this, &Interface::quitGameClicked);

    m_game.startGame();
    updateChessboard();

    // Set window options
    setFixedSize(minimumSizeHint());
    setWindowTitle("Chess");
}

Interface::~Interface()
{
    delete ui;
}

void Interface::initializeChessboard(QGridLayout *layout)
{
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            m_squares[row][col] = new ChessSquare(this);
            m_squares[row][col]->setMinimumSize(60, 60);
            m_squares[row][col]->setAlignment(Qt::AlignCenter);
            m_squares[row][col]->setFrameStyle(QFrame::Panel | QFrame::Sunken);
            layout->addWidget(m_squares[row][col], row, col); // Use the passed layout

            // Set background color (chessboard pattern)
            if ((row + col) % 2 == 0) {
                m_squares[row][col]->setStyleSheet("background-color: white;");
            } else {
                m_squares[row][col]->setStyleSheet("background-color: lightgray;");
            }

            // Connect the custom signal
            QString position = QString(QChar('a' + col)) + QString::number(8 - row);
            m_squares[row][col]->setPosition(position);
            connect(m_squares[row][col], &ChessSquare::squareClicked, this, &Interface::squareClicked);
        }
    }
}

void Interface::updateChessboard()
{
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QString position = m_squares[row][col]->position();
            Figure* figure = m_game.getFigureAt(position);
            if (figure) {
                QString iconPath = "://icons/";
                iconPath += QString::fromStdString(figure->getColor() == Color::WHITE ? "w" : "b");
                iconPath += figure->getTypeString() + ".png";
                m_squares[row][col]->setPixmap(QPixmap(iconPath).scaled(QSize(50, 50), Qt::KeepAspectRatio));
            } else {
                m_squares[row][col]->clear();
            }
        }
    }
}

void Interface::squareClicked(ChessSquare *square) {
    if (!square)
        return;

    QString position = square->position();

    // Click handling logic
    // 1. If a piece is selected and the clicked square is a valid move, make the move.
    // 2. If no piece is selected, select the piece (if it exists).
    // 3. Update the interface (updateChessboard, highlightPossibleMoves).

    static QString selectedPosition = "";
    if (selectedPosition.isEmpty()) {
        Figure* clicked = m_game.getFigureAt(position);
        if (clicked && m_game.currentPlayerColor() == clicked->getColor()) {
            selectedPosition = position;
            highlightPossibleMoves(selectedPosition);
        }
    } else {
        QString move = selectedPosition + " " + position;
        if (m_game.isValidMove(selectedPosition, position)) {
            m_game.makeMove(move);
            updateChessboard();
        }
        clearSelection();
        selectedPosition = "";
    }
}

void Interface::newGameClicked()
{
    m_game.startGame();
    updateChessboard();
}

void Interface::quitGameClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Are you sure you want to quit?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void Interface::clearSelection() {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if ((row + col) % 2 == 0) {
                m_squares[row][col]->setStyleSheet("background-color: white;");
            } else {
                m_squares[row][col]->setStyleSheet("background-color: lightgray;");
            }
        }
    }
}

void Interface::highlightPossibleMoves(const QString& position) {
    QVector<QString> moves = m_game.getAvailableMovesForFigure(position);
    for (const QString& move : moves) {
        // Highlight the possible move squares
        int row = 8 - (move[1].digitValue());
        int col = move[0].toLatin1() - 'a';
        m_squares[row][col]->setStyleSheet("background-color: yellow;");
    }
}

QString Interface::getPositionFromSquare(ChessSquare *square) const {
    if (!square)
        return "";
    return square->position();
}

Figure* Interface::promoteWindow(Color color) {
    QDialog dialog(nullptr);
    dialog.setWindowTitle("Pawn Promotion");
    QGridLayout layout(&dialog);

    // Figure options (Queen, Rook, Bishop, Knight)
    QStringList figureTypes = {"queen", "rook", "bishop", "knight"};
    Figure* selectedFigure = nullptr;

    for (int i = 0; i < figureTypes.size(); ++i) {
        QString figureType = figureTypes[i];
        // Construct the image path based on color and type
        QString imagePath = QString("://icons/");
        imagePath += (color == Color::WHITE ? "w" : "b") + figureType + ".png";
        QPixmap pixmap(imagePath);
        if (pixmap.isNull()) {
            qDebug() << "Error loading image:" << imagePath;
            continue; // Skip if image loading fails
        }
        QPushButton* button = new QPushButton();
        // Scale the pixmap to 100x100
        QPixmap scaledPixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio);
        button->setIcon(QIcon(scaledPixmap));
        button->setIconSize(scaledPixmap.size());
        layout.addWidget(button, i / 2, i % 2); // Arrange in a grid

        // Connect button click to promotion logic
        QObject::connect(button, &QPushButton::clicked, [=, &dialog, &selectedFigure]() {
            if (figureType == "queen") {
                selectedFigure = new Queen(color);
            } else if (figureType == "rook") {
                selectedFigure = new Rook(color);
            } else if (figureType == "bishop") {
                selectedFigure = new Bishop(color);
            } else if (figureType == "knight") {
                selectedFigure = new Knight(color);
            }
            dialog.accept(); // Close the dialog
        });
    }

    dialog.exec(); // Show the dialog
    return selectedFigure;
}

void Interface::endGamePopup(Player* winner) {
    QString message;
    if (winner == nullptr) {
        message = "The game is a draw!";
    } else if (winner->getColor() == Color::WHITE) {
        message = "White wins!";
    } else {
        message = "Black wins!";
    }

    QMessageBox::information(nullptr, "Game Over", message);
    // nullptr as parent, so the message box appears as a top-level window
}
