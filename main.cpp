#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w; // plus de paramètre ici
    w.show();
    return a.exec();
}
