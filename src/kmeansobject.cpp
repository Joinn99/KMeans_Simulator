/*       KMeans Simulator       */
/*        Author: 卫天峻         */
/*       kmeansobject.h         */
/*   描述： K-Means 逻辑对象实现   */
#include "kmeansobject.h"

/*         描述：对象创建         */
KMeansObject::KMeansObject(QObject *parent, int patNumber, int cluNumber, QPoint bound) : QObject(parent)
{
    //初始化
    vectorPatPoint.resize(patNumber);
    vectorPatGroup.resize(patNumber);
    vectorProtoPoint.resize(cluNumber);
    vectorProtoCount.resize(cluNumber);
    vectorCompactness.resize(cluNumber);
    vectorCriterion.resize(4);
    //生成使样本点聚集的伪“簇中心”
    for(int j = 0; j < vectorProtoPoint.size(); j++){
        vectorProtoPoint[j] = QPointF(50 + qrand() % (bound.x() - 100),
                                      50 + qrand() % (bound.y() - 100));
    }
    //生成样本点数据集
    for(int i = 0; i < vectorPatPoint.size(); i++){
        vectorPatPoint[i] = pointGenerator(bound);
    }
    //在样本点中随机选取k个作为簇中心
    for(int j = 0; j < vectorProtoPoint.size(); j++){
        vectorProtoPoint[j] = vectorPatPoint[j];
    }
    updatePatGroup();       //根据簇中心确定样本点所属簇
    criterionUpdate();      //更新评估指标

    //设置计时器，1秒迭代一次
    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(clusterUpdate()));
    updateTimer.setInterval(1000);
    updateTimer.start();
}

/*         描述：暂停迭代         */
void KMeansObject::pause(bool flagPause)
{
    //停止计时器
    if(flagPause){updateTimer.stop();}
    //开始计时器
    else{updateTimer.start();}
}

/*        描述：更新簇中心        */
void KMeansObject::updatePrototype()
{
    //归零
    vectorProtoPoint.fill(QPointF(0,0));
    //计算簇内样本点坐标之和
    for(int i = 0; i < vectorPatPoint.size(); i++){
        vectorProtoPoint[vectorPatGroup[i]] += vectorPatPoint[i];
    }
    //求得坐标平均值，更新簇中心
    for(int j = 0; j < vectorProtoPoint.size(); j++){
        vectorProtoPoint[j] /= vectorProtoCount[j];
    }
}

/*       描述：更新样本所属簇      */
bool KMeansObject::updatePatGroup()
{
    //保存上一代样本点所属簇信息
    QVector<int> currentPatGroup = vectorPatGroup;
    //归零
    vectorCompactness.fill(0);
    vectorProtoCount.fill(0);
    //更新每个样本点所属簇
    for(int i = 0; i < vectorPatPoint.size(); i++){
        //计算样本点到各个簇中心距离，选出最小距离
        double minDistance = qInf();
        int minPatGroup = 0;
        for(int j = 0; j < vectorProtoPoint.size(); j++){
            double distance = (vectorPatPoint[i] - vectorProtoPoint[j]).manhattanLength();
            if(distance < minDistance){
                minPatGroup = j;
                minDistance = distance;
            }
        }
        //更新样本所属簇
        vectorCompactness[minPatGroup] += minDistance;
        vectorPatGroup[i] = minPatGroup;
        vectorProtoCount[vectorPatGroup[i]] += 1;
    }
    //判断新的样本点所属簇与上一代相比是否变化
    bool groupChanged = false;
    for(int j = 0; j < vectorPatGroup.size(); j++){
        if(vectorPatGroup[j] != currentPatGroup[j]){
            groupChanged = true;
        }
    }
    return groupChanged;
}

/*         描述：迭代控制         */
void KMeansObject::clusterUpdate()
{
    updatePrototype();  //更新簇中心
    if(updatePatGroup()){   //更新样本所属簇，判断是否中止迭代
        //继续迭代
        criterionUpdate();  //更新评估指标
        emit clusterUpdated();  //通知界面更新
    }
    else{
        //中止迭代
        criterionUpdate();  //更新评估指标
        emit clusterFinished(); //通知界面结束更新
        updateTimer.stop(); //停止定时器
    }
}

/*         描述：返回评估指标      */
QVector<double> KMeansObject::criterion()
{
    return vectorCriterion;
}

/*      描述：返回对应样本点坐标    */
QPointF KMeansObject::pattern(int index)
{
    return vectorPatPoint[index];
}

/*      描述：返回对应簇中心坐标    */
QPointF KMeansObject::prototype(int index)
{
    return vectorProtoPoint[index];
}

/*        描述：返回样本数        */
int KMeansObject::patNumber()
{
    return vectorPatPoint.size();
}

/*         描述：返回簇数         */
int KMeansObject::protoNumber()
{
    return vectorProtoPoint.size();
}

/*     描述：返回对应样本所属簇     */
int KMeansObject::patGroup(int index)
{
    return vectorPatGroup[index];
}

/*      描述：随机初始化样本点      */
QPointF KMeansObject::pointGenerator(QPoint bound)
{
    //使用之前初始化的伪“簇中心”为中心生成样本点，使得生成的样本集不是均匀分布，
    //而是聚集在数个中心附近，以增强聚类的实验效果
    double pointx = qPow(double(qrand() % 10000) / 10000 *
                             qSqrt(bound.x()/vectorProtoPoint.size()),2) / 2;
    double pointy = qPow(double(qrand() % 10000) / 10000 *
                             qSqrt(bound.y()/vectorProtoPoint.size()),2) / 2;
    QPointF point = vectorProtoPoint[qrand() % vectorProtoPoint.size()] +
                                QPointF((qrand() % 2) ? pointx : -pointx,
                                        (qrand() % 2) ? pointy : -pointy);
    if(point.x() > bound.x()){point.rx() -= 2 * pointx;}
    else if(point.x() < 0){point.rx() += 2 * pointx;}
    if(point.y() > bound.y()){point.ry() -= 2 * pointy;}
    else if(point.y() < 0){point.ry() += 2 * pointy;}
    return point;
}

/*        描述：评估指标更新       */
void KMeansObject::criterionUpdate()
{
    //归零
    vectorCriterion[COMP] = 0;
    vectorCriterion[SEPR] = 0;
    vectorCriterion[DBI] = 0;
    //计算紧致度
    for(int i = 0; i < vectorProtoPoint.size(); i++){
        vectorCriterion[COMP] += vectorCompactness[i];
        vectorCompactness[i] /= vectorProtoCount[i];
    }
    vectorCriterion[COMP] /= vectorPatPoint.size();
    //计算间隔度和DBI
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
    //迭代次数+1
    vectorCriterion[EPOCH] += 1;
}

