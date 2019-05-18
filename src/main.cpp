/*       KMeans Simulator       */
/*        Author: 卫天峻         */
/*          main.cpp            */
/*         描述：主程序           */
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("K-Means Simulator");
    w.show();

    return a.exec();
}
