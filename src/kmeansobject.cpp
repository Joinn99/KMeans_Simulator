#include "kmeansobject.h"

KMeansObject::KMeansObject(QObject *parent, int patNumber, int cluNumber, QPoint bound) : QObject(parent)
{
    vectorPatPoint.resize(patNumber);
    vectorPatGroup.resize(patNumber);
    vectorProtoPoint.resize(cluNumber);
    vectorProtoCount.resize(cluNumber);
    vectorCompactness.resize(cluNumber);
    vectorCriterion.resize(4);
    for(int j = 0; j < vectorProtoPoint.size(); j++){
        vectorProtoPoint[j] = QPointF(QRandomGenerator::global()->bounded(50, bound.x() - 50),
                                      QRandomGenerator::global()->bounded(50, bound.y() - 50));
    }
    for(int i = 0; i < vectorPatPoint.size(); i++){
        vectorPatPoint[i] = pointGenerator(bound);
    }
    for(int j = 0; j < vectorProtoPoint.size(); j++){
        vectorProtoPoint[j] = vectorPatPoint[j];
    }
    updatePatGroup();
    criterionUpdate();
    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(clusterUpdate()));
    updateTimer.setInterval(1000);
    updateTimer.start();
}

void KMeansObject::pause(bool flagPause)
{
    if(flagPause){updateTimer.stop();}
    else{updateTimer.start();}
}

void KMeansObject::updatePrototype()
{
    vectorProtoPoint.fill(QPointF(0,0));
    for(int i = 0; i < vectorPatPoint.size(); i++){
        vectorProtoPoint[vectorPatGroup[i]] += vectorPatPoint[i];
    }
    for(int j = 0; j < vectorProtoPoint.size(); j++){
        vectorProtoPoint[j] /= vectorProtoCount[j];
    }
}

bool KMeansObject::updatePatGroup()
{
    QVector<int> currentPatGroup = vectorPatGroup;
    vectorCompactness.fill(0);
    vectorProtoCount.fill(0);
    for(int i = 0; i < vectorPatPoint.size(); i++){
        double minDistance = qInf();
        int minPatGroup = 0;
        for(int j = 0; j < vectorProtoPoint.size(); j++){
            double distance = (vectorPatPoint[i] - vectorProtoPoint[j]).manhattanLength();
            if(distance < minDistance){
                minPatGroup = j;
                minDistance = distance;
            }
        }
        vectorCompactness[minPatGroup] += minDistance;
        vectorPatGroup[i] = minPatGroup;
        vectorProtoCount[vectorPatGroup[i]] += 1;
    }
    bool groupChanged = false;
    for(int j = 0; j < vectorPatGroup.size(); j++){
        if(vectorPatGroup[j] != currentPatGroup[j]){
            groupChanged = true;
        }
    }
    return groupChanged;
}

void KMeansObject::clusterUpdate()
{
    updatePrototype();
    if(updatePatGroup()){
        criterionUpdate();
        emit clusterUpdated();
    }
    else{
        criterionUpdate();
        emit clusterFinished();
        updateTimer.stop();
    }
}

QVector<double> KMeansObject::criterion()
{
    return vectorCriterion;
}

QPointF KMeansObject::pattern(int index)
{
    return vectorPatPoint[index];
}

QPointF KMeansObject::prototype(int index)
{
    return vectorProtoPoint[index];
}

int KMeansObject::patNumber()
{
    return vectorPatPoint.size();
}

int KMeansObject::protoNumber()
{
    return vectorProtoPoint.size();
}

int KMeansObject::patGroup(int index)
{
    return vectorPatGroup[index];
}

QPointF KMeansObject::pointGenerator(QPoint bound)
{
    double pointx = qPow(QRandomGenerator::global()->bounded(
                             qSqrt(bound.x()/vectorProtoPoint.size())),2) / 2;
    double pointy = qPow(QRandomGenerator::global()->bounded(
                             qSqrt(bound.y()/vectorProtoPoint.size())),2) / 2;
    QPointF point = vectorProtoPoint[QRandomGenerator::global()->bounded(vectorProtoPoint.size())] +
                                QPointF(QRandomGenerator::global()->bounded(2) ? pointx : -pointx,
                                        QRandomGenerator::global()->bounded(2) ? pointy : -pointy);
    if(point.x() > bound.x()){point.rx() -= 2 * pointx;}
    else if(point.x() < 0){point.rx() += 2 * pointx;}
    if(point.y() > bound.y()){point.ry() -= 2 * pointy;}
    else if(point.y() < 0){point.ry() += 2 * pointy;}
    return point;
}

void KMeansObject::criterionUpdate()
{
    vectorCriterion[COMP] = 0;
    vectorCriterion[SEPR] = 0;
    vectorCriterion[DBI] = 0;
    for(int i = 0; i < vectorProtoPoint.size(); i++){
        vectorCriterion[COMP] += vectorCompactness[i];
        vectorCompactness[i] /= vectorProtoCount[i];
    }
    vectorCriterion[COMP] /= vectorPatPoint.size();
    for(int i = 0; i < vectorProtoPoint.size(); i++){
        double maxDb = 0;
        for(int j = 0; j < vectorProtoPoint.size(); j++){
            double distanceIJ = QPointF(vectorProtoPoint[i] - vectorProtoPoint[j]).manhattanLength();
            vectorCriterion[SEPR] += distanceIJ;
            if(i!=j){maxDb = qMax(maxDb, (vectorCompactness[i] + vectorCompactness[j]) / distanceIJ);}
        }
        vectorCriterion[DBI] += maxDb;
    }
    vectorCriterion[SEPR] /= (vectorProtoPoint.size() * (vectorProtoPoint.size()-1)) / 2;
    vectorCriterion[DBI] /= vectorProtoPoint.size();
    vectorCriterion[EPOCH] += 1;
}

