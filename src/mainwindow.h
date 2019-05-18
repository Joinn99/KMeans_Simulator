/*       KMeans Simulator       */
/*        Author: 卫天峻         */
/*        mainwindow.h          */
/*      描述：主窗口对象定义       */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include "kmeansdisplay.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void start();
    void finish();
    void updateWindow(QVector<double> criterion);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
