#include "mainwindow.h"
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QStringListModel>
#include <QScrollBar>
#include <QTreeWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    // Set up the UI
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    textEdit = new QTextEdit(this);
    trieVisualization = new QTreeWidget(this);
    trieVisualization->setHeaderLabel("Trie Visualization");
    trieVisualization->setVisible(false); // Hide visualization by default

    visualizationCheckbox = new QCheckBox("Show Trie Visualization", this);
    visualizationCheckbox->setChecked(false); // Unchecked by default

    layout->addWidget(textEdit);
    layout->addWidget(visualizationCheckbox);
    layout->addWidget(trieVisualization);
    setCentralWidget(centralWidget);

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
    connect(visualizationCheckbox, &QCheckBox::toggled, this, &MainWindow::toggleVisualization);

    // Install event filter to handle key events
    textEdit->installEventFilter(this);
}

MainWindow::~MainWindow() {}

void MainWindow::onTextChanged() {
    QString currentWord = getCurrentWord();

    if (currentWord.isEmpty()) {
        completer->popup()->hide();
        trieVisualization->clear(); // Clear Trie visualization
        return;
    }

    // Update the completer's model
    updateCompleterModel();

    // Show the completer popup
    QRect cursorRect = textEdit->cursorRect();
    cursorRect.setWidth(completer->popup()->sizeHintForColumn(0)
                        + completer->popup()->verticalScrollBar()->sizeHint().width());
    completer->complete(cursorRect); // Show the popup

    // Visualize Trie search if the checkbox is checked
    if (visualizationCheckbox->isChecked()) {
        visualizeTrieSearch(currentWord);
    }
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

void MainWindow::toggleVisualization(bool checked) {
    trieVisualization->setVisible(checked); // Show/hide the visualization
    if (checked) {
        // Update the visualization if the checkbox is checked
        QString currentWord = getCurrentWord();
        if (!currentWord.isEmpty()) {
            visualizeTrieSearch(currentWord);
        }
    }
}

void MainWindow::visualizeTrieSearch(const QString &prefix) {
    trieVisualization->clear(); // Clear previous visualization

    // Traverse the Trie based on the prefix
    TrieNode *current = trie.getRoot();
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(trieVisualization);
    rootItem->setText(0, "Root");

    for (QChar ch : prefix) {
        if (current->children.find(ch.toLatin1()) == current->children.end()) {
            return; // Prefix not found
        }
        current = current->children[ch.toLatin1()];

        QTreeWidgetItem *childItem = new QTreeWidgetItem(rootItem);
        childItem->setText(0, QString(ch));
        rootItem = childItem;
    }

    // Build the visualization for the remaining Trie nodes
    buildTrieVisualization(current, rootItem);
    trieVisualization->expandAll(); // Expand all nodes for better visibility
}

void MainWindow::buildTrieVisualization(TrieNode *node, QTreeWidgetItem *parent) {
    for (auto &child : node->children) {
        QTreeWidgetItem *childItem = new QTreeWidgetItem(parent);
        childItem->setText(0, QString(child.first));

        // Recursively build the visualization for child nodes
        buildTrieVisualization(child.second, childItem);
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == textEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        // Handle Enter key for autocompletion
        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
            if (completer->popup()->isVisible()) {
                completer->complete(); // Trigger autocompletion
                return true; // Event handled
            }
        }
    }

    // Pass the event to the base class
    return QMainWindow::eventFilter(obj, event);
}