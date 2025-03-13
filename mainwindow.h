#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QCompleter>
#include <QStringList>
#include "trie.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override; // Event filter for key handling

    private slots:
        void onTextChanged(); // Slot for text changes
    void insertCompletion(const QString &completion); // Slot for inserting completion

private:
    QTextEdit *textEdit; // Notepad-like text area
    QCompleter *completer; // Autocomplete dropdown
    QStringList wordList; // List of words for autocomplete
    Trie trie; // Trie for word storage and retrieval

    void updateCompleterModel(); // Update the completer's model
    QString getCurrentWord() const; // Get the current word being typed
};

#endif // MAINWINDOW_H