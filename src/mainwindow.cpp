#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFile file(QString(":/theme/res/Default-0.qss"));
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();
    ui->labelLogo->setPixmap(QPixmap(":/icon/res/logo.png"));
    connect(ui->toolCreate, SIGNAL(clicked(bool)), this, SLOT(start()));
    connect(ui->widget, SIGNAL(updateCriterion(QVector<double>)), this, SLOT(updateWindow(QVector<double>)));
    connect(ui->widget, SIGNAL(updateFinished()), this, SLOT(finish()));
    connect(ui->toolPause, SIGNAL(clicked(bool)), ui->widget, SLOT(runContorol(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start()
{
    ui->widget->createCluster(ui->spinPat->value(), ui->spinClu->value());
    ui->toolPause->setEnabled(true);
    ui->toolCreate->setEnabled(false);
}

void MainWindow::finish()
{
    ui->toolCreate->setEnabled(true);
    ui->toolPause->setEnabled(false);
}

void MainWindow::updateWindow(QVector<double> criterion)
{
    ui->labelCP->setText(QString("Compactness: %1").arg(criterion.value(0), 5));
    ui->labelSP->setText(QString("Seperation: %1").arg(criterion.value(1), 5));
    ui->labelDBI->setText(QString("DBI: %1").arg(criterion.value(2), 5));
    ui->labelEpoch->setText(QString("Epoch: %1").arg(int(criterion.value(3))));
}
