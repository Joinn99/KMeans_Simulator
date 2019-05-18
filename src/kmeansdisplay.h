/*       KMeans Simulator       */
/*        Author: 卫天峻         */
/*       kmeansdisplay.h        */
/*  描述： K-Means 显示对象定义    */
#ifndef KMEANSDISPLAY_H
#define KMEANSDISPLAY_H

#include <QWidget>
#include <QPainter>
#include "kmeansobject.h"

class KMeansDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit KMeansDisplay(QWidget *parent = nullptr);
    void createCluster(int patNumber, int cluNumber);
public slots:
    void runContorol(bool flag);
    void finishContorol();
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
signals:
    void updateCriterion(QVector<double>);
    void updateFinished();
private:
    QPainter* painter;
    KMeansObject* object;
    QVector<QPen> pens;
    QPixmap background;
};

#endif // KMEANSDISPLAY_H
