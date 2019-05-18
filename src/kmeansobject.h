/*       KMeans Simulator       */
/*        Author: 卫天峻         */
/*       kmeansobject.h         */
/*   描述： K-Means 逻辑对象定义   */
#ifndef KMEANSOBJECT_H
#define KMEANSOBJECT_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include <QtMath>
#include <QPointF>

class KMeansObject : public QObject
{
    Q_OBJECT
public:
    explicit KMeansObject(QObject *parent = nullptr, int patNumber=50, int cluNumber=5, QPoint bound=QPoint());
    void pause(bool);
    void updatePrototype();
    bool updatePatGroup();
    QVector<double> criterion();
    QPointF pattern(int index);
    QPointF prototype(int index);
    int patNumber();
    int protoNumber();
    int patGroup(int index);
signals:
    void clusterFinished();
    void clusterUpdated();
public slots:
    void clusterUpdate();
private:
    enum{COMP, SEPR, DBI, EPOCH};           //枚举：聚类评估指标
    QVector<QPointF> vectorPatPoint;        //变量：样本点坐标
    QVector<int> vectorPatGroup;            //变量：样本点簇类别
    QVector<QPointF> vectorProtoPoint;      //变量：簇中心坐标
    QVector<int> vectorProtoCount;          //变量：簇内样本点数
    QVector<double> vectorCompactness;      //变量：簇紧致度
    QVector<double> vectorCriterion;        //变量：聚类评估指标
    QTimer updateTimer;                     //对象：定时器
    
    QPointF pointGenerator(QPoint bound);
    void criterionUpdate();
};


#endif // KMEANSOBJECT_H
