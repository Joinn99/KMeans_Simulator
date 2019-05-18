/*       KMeans Simulator       */
/*        Author: 卫天峻         */
/*      kmeansdisplay.cpp       */
/*  描述： K-Means 显示对象实现    */
#include "kmeansdisplay.h"

/*        描述：对象创建          */
KMeansDisplay::KMeansDisplay(QWidget *parent) : QWidget(parent)
{
    background = QPixmap(":/icon/res/Coord.png");
    object = nullptr;
    setUpdatesEnabled(true);
    pens.resize(5);
    for(int i = 0; i < pens.size(); i++){
        pens[i] = QPen(QColor::fromHsv(70 * i, 255, 255),10,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin);
    }
}

/*        描述：对象销毁         */
void KMeansDisplay::runContorol(bool flag)
{
    if(object){
        object->pause(flag);
    }
}

/*        描述：运行结束          */
void KMeansDisplay::finishContorol()
{
    update();
    emit updateFinished();
}

/*       描述：绘制聚类图像        */
void KMeansDisplay::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawPixmap(0, 0, 512, 512, background);
    if(object){
        // 绘制样本点
        for(int i = 0; i< object->patNumber(); i++){
            painter.setPen(pens[object->patGroup(i)]);
            painter.drawPoint(object->pattern(i));
        }
        //绘制聚类中心
        painter.setPen(QPen(QColor::fromRgba(qRgba(255,255,255,168)),20,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
        for(int j = 0; j< object->protoNumber(); j++){
            painter.drawPoint(object->prototype(j));
        }
        updateCriterion(object->criterion());
    }
}

/*      描述：初始化聚类数据集       */
void KMeansDisplay::createCluster(int patNumber, int cluNumber)
{
    if(object){
        object->~KMeansObject();
    }
    object = new KMeansObject(this, patNumber, cluNumber, rect().bottomRight());
    connect(object, SIGNAL(clusterUpdated()), this, SLOT(update()));
    connect(object, SIGNAL(clusterFinished()), this, SLOT(finishContorol()));
    update();
}
