#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("BabeDB——数据库系统实现作品");
    w.show();

    return a.exec();
}
