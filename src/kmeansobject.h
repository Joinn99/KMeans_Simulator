#ifndef KMEANSOBJECT_H
#define KMEANSOBJECT_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include <QtMath>
#include <QPointF>
#include <QRandomGenerator>

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
    enum{COMP, SEPR, DBI, EPOCH};
    QVector<QPointF> vectorPatPoint;
    QVector<int> vectorPatGroup;
    QVector<QPointF> vectorProtoPoint;
    QVector<int> vectorProtoCount;
    QVector<double> vectorCompactness;
    QVector<double> vectorCriterion;
    QTimer updateTimer;
    QPointF pointGenerator(QPoint bound);
    void criterionUpdate();
};


#endif // KMEANSOBJECT_H
