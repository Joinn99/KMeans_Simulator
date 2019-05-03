#include "kmeansdisplay.h"

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

void KMeansDisplay::runContorol(bool flag)
{
    if(object){
        object->pause(flag);
    }
}

void KMeansDisplay::finishContorol()
{
    update();
    emit updateFinished();
}

void KMeansDisplay::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawPixmap(0, 0, 512, 512, background);
    if(object){
        for(int i = 0; i< object->patNumber(); i++){
            painter.setPen(pens[object->patGroup(i)]);
            painter.drawPoint(object->pattern(i));
        }
        painter.setPen(QPen(QColor::fromRgba(qRgba(255,255,255,168)),20,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
        for(int j = 0; j< object->protoNumber(); j++){
            painter.drawPoint(object->prototype(j));
        }
        updateCriterion(object->criterion());
    }
}

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
