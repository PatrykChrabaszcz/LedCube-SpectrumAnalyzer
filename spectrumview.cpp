#include "spectrumview.h"
#include <QPainter>
#include <QRect>
#include <QDebug>
int cubeSize=64;
SpectrumView::SpectrumView(QWidget *parent) :
    QWidget(parent)
{
    spectrumData=NULL;
}
void SpectrumView::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    painter.setPen(Qt::red);
    painter.fillRect(rect(), Qt::black);

    //To keep spectrum in the middle;
    int correction=0;
    int widthTemp=width();
    while(widthTemp%cubeSize){
        widthTemp--;
        correction++;
    }
    correction /=2;
    int barWidth=widthTemp/cubeSize;

    if(spectrumData!=NULL){
        for(int i=0;i<cubeSize;i++){
            QRect bar;
            bar.setRect(correction+i*barWidth,height(),barWidth,-1*spectrumData[i]*height());
            painter.drawRect(bar);
        }
    }
    update();
}
void SpectrumView::updateSpectrumData(double *data,int size){
    if(size!=cubeSize)
        qDebug()<<"Error";
    else
        spectrumData=data;
}
