#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // Initialize the Qt application
    MainWindow window; // Create an instance of your main window
    window.show(); // Show the main window
    return app.exec(); // Start the Qt event loop
}