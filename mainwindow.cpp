#include "mainwindow.h"
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QStringListModel>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    // Set up the UI
    textEdit = new QTextEdit(this);
    setCentralWidget(textEdit); // Set textEdit as the central widget

    // Initialize the completer
    completer = new QCompleter(this);
    completer->setCaseSensitivity(Qt::CaseInsensitive); // Case-insensitive matching
    completer->setCompletionMode(QCompleter::PopupCompletion); // Show suggestions as a dropdown
    completer->setWidget(textEdit); // Attach completer to the textEdit

    // Insert sample words into the Trie and wordList
    wordList << "predict" << "prevent" << "prepare" << "program" << "project" << "print" << "public" << "private";
    for (const QString &word : wordList) {
        trie.insert(word.toStdString());
    }

    // Connect signals and slots
    connect(textEdit, &QTextEdit::textChanged, this, &MainWindow::onTextChanged);
    connect(completer, QOverload<const QString &>::of(&QCompleter::activated), this, &MainWindow::insertCompletion);

    // Install event filter to handle key events
    textEdit->installEventFilter(this);
}

MainWindow::~MainWindow() {}

void MainWindow::onTextChanged() {
    QString currentWord = getCurrentWord();

    if (currentWord.isEmpty()) {
        completer->popup()->hide();
        return;
    }

    // Update the completer's model
    updateCompleterModel();

    // Show the completer popup
    QRect cursorRect = textEdit->cursorRect();
    cursorRect.setWidth(completer->popup()->sizeHintForColumn(0)
                        + completer->popup()->verticalScrollBar()->sizeHint().width());
    completer->complete(cursorRect); // Show the popup
}

void MainWindow::updateCompleterModel() {
    QString currentWord = getCurrentWord();

    // Retrieve suggestions from the Trie
    std::vector<std::string> trieSuggestions = trie.search(currentWord.toStdString());
    QStringList suggestions;
    for (const std::string &word : trieSuggestions) {
        suggestions << QString::fromStdString(word);
    }

    // Update the completer's model
    completer->setModel(new QStringListModel(suggestions, completer));
}

QString MainWindow::getCurrentWord() const {
    QTextCursor cursor = textEdit->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    return cursor.selectedText();
}

void MainWindow::insertCompletion(const QString &completion) {
    QTextCursor cursor = textEdit->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    cursor.insertText(completion);
    completer->popup()->hide(); // Hide the popup after insertion
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == textEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (completer->popup()->isVisible()) {
            if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
                QString completion = completer->currentCompletion();
                if (!completion.isEmpty()) {
                    insertCompletion(completion);
                    return true;  // Stop further processing, preventing newline insertion
                }
            }
        }
    }

    return QMainWindow::eventFilter(obj, event);
}



