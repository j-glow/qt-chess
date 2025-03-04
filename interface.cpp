#include "interface.h"
#include "ui_interface.h"

#include <QMessageBox>

Interface::Interface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Interface)
{
    ui->setupUi(this);

    // Initialize the interface
    m_chessboardLayout = new QGridLayout;
    initializeChessboard();

    // Control buttons
    m_newGameButton = new QPushButton("New Game", this);
    m_quitGameButton = new QPushButton("Quit", this);

    // Window layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addLayout(m_chessboardLayout);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(m_newGameButton);
    buttonLayout->addWidget(m_quitGameButton);
    mainLayout->addLayout(buttonLayout);
    setCentralWidget(centralWidget);

    // Connect signals and slots
    connect(m_newGameButton, &QPushButton::clicked, this, &Interface::newGameClicked);
    connect(m_quitGameButton, &QPushButton::clicked, this, &Interface::quitGameClicked);

    m_game.startGame(); // Start a new game
    updateChessboard(); // Update the display
}

Interface::~Interface()
{
    delete ui; // Delete UI pointer if using Qt Designer
}

void Interface::initializeChessboard()
{
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            m_squares[row][col] = new QLabel(this);
            m_squares[row][col]->setMinimumSize(60, 60); // Set minimum square size
            m_squares[row][col]->setAlignment(Qt::AlignCenter); // Center the content
            m_squares[row][col]->setFrameStyle(QFrame::Panel | QFrame::Sunken); // Set square border style
            m_chessboardLayout->addWidget(m_squares[row][col], row, col);

            // Set background color (chessboard pattern)
            if ((row + col) % 2 == 0) {
                m_squares[row][col]->setStyleSheet("background-color: white;");
            } else {
                m_squares[row][col]->setStyleSheet("background-color: lightgray;");
            }

            // Connect click signal
            connect(m_squares[row][col], &QLabel::mousePressEvent, [this, row, col](QMouseEvent* event){
                Q_UNUSED(event); // Avoid unused variable warning
                this->squareClicked();
            });
        }
    }
}

void Interface::updateChessboard()
{
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QString position = getPositionFromSquare(m_squares[row][col]);
            Figure* figure = m_game.m_chessboard.getFigureAt(position); // Get the figure at the position
            if (figure) {
                QString iconPath = ":/icons/"; // Path to your icon resources (adjust as needed)
                iconPath += QString::fromStdString(figure->getColor() == Color::WHITE ? "w" : "b"); // Figure color
                iconPath += QString::fromStdString(figure->getTypeString()) + ".png"; // Figure type (e.g., "pawn", "rook")
                m_squares[row][col]->setPixmap(QPixmap(iconPath).scaled(QSize(50, 50), Qt::KeepAspectRatio)); // Display the icon
            } else {
                m_squares[row][col]->clear(); // Clear the square if there's no figure
            }
        }
    }
}

void Interface::squareClicked() {
    QLabel* clickedSquare = qobject_cast<QLabel*>(sender()); // Get the clicked square

    if (!clickedSquare) return;

    QString position = getPositionFromSquare(clickedSquare);

    // Click handling logic
    // 1. If a piece is selected and the clicked square is a valid move, make the move.
    // 2. If no piece is selected, select the piece (if it exists).
    // 3. Update the interface (updateChessboard, highlightPossibleMoves).

    static QString selectedPosition = "";
    if (selectedPosition.isEmpty()) {
        if (m_game.m_chessboard.getFigureAt(position) != nullptr) {
            selectedPosition = position;
            highlightPossibleMoves(selectedPosition);
        }
    } else {
        QString move = selectedPosition + " " + position;
        if (m_game.isValidMove(move)) {
            m_game.makeMove(move);
            updateChessboard();
        }
        clearSelection();
        selectedPosition = "";
    }
}

void Interface::newGameClicked()
{
    m_game.endGame();
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
    QVector<QString> moves = m_game.m_chessboard.getAvailableMovesForFigure(position);
    for (const QString& move : moves) {
        // Highlight the possible move squares
        int row = 8 - (move[1].digitValue());
        int col = move[0].toLatin1() - 'a';
        m_squares[row][col]->setStyleSheet("background-color: yellow;");
    }
}

QString Interface::getPositionFromSquare(QLabel *square) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (m_squares[row][col] == square) {
                char file = 'a' + col;
                char rank = '8' - row;
                return QString(QChar(file)) + QString::number(rank);
            }
        }
    }
    return ""; // Return an empty string if not found
}
