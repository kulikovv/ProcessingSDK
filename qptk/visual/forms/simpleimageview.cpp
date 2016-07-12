#include "simpleimageview.h"
#include <QFileDialog>
#include <QAction>
#include <QPainter>
#include <QMouseEvent>
#include <types/rgb.h>

using namespace visual::ui;

SimpleImageView::SimpleImageView(QWidget *parent) :
    QWidget(parent)
{

  //  this->setStyleSheet("background-color: rgb(0, 0, 0);");
    this->setMinimumSize(320,240);
    this->setMaximumSize(64000,64000);
    QAction* saveShot = new QAction(QIcon(":/sdk/visual/forms/SaveImage"),tr("Save image"),this);
    connect(saveShot,SIGNAL(triggered()),this,SIGNAL(saveScreen()));
    this->addAction(saveShot);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
    _resized = QSize(0,0);
}
void SimpleImageView::setData(QImage* var)
{
    m_im = *var;
    _originalSize = m_im.size();
    this->update();
}
void SimpleImageView::mousePressEvent( QMouseEvent * event)
{
    if ( event->buttons() ==   Qt::LeftButton  )
    {


        int dw = (this->width()-_resized.width())/2;
        int dh = (this->height()-_resized.height())/2;
        QRect rect;
        rect.setRect(dw,dh,_resized.width(),_resized.height());
        if(rect.contains(event->pos()))
        {
            QPoint coordImage = event->pos()-rect.topLeft();
            QPointF coordRealimage((float)coordImage.x()/(float)_resized.width(),(float)coordImage.y()/(float)_resized.height());
            MouseControl mc;
            mc.button = event->buttons();
            mc.globalCoordinates = event->globalPos();
            mc.widgetCoordinates = event->pos();
            mc.imgCoordinates = coordRealimage;
            emit imageClicked(mc);
        }
    }
}

void SimpleImageView::mouseMoveEvent(QMouseEvent *event)
{

    int dw = (this->width()-_resized.width())/2;
    int dh = (this->height()-_resized.height())/2;
    QRect rect;
    rect.setRect(dw,dh,_resized.width(),_resized.height());
    if(rect.contains(event->pos()))
    {
        QPoint coordImage = event->pos()-rect.topLeft();
        QPointF coordRealimage((float)coordImage.x()/(float)_resized.width(),(float)coordImage.y()/(float)_resized.height());
        MouseControl mc;
        mc.button = event->buttons();
        mc.globalCoordinates = event->globalPos();
        mc.widgetCoordinates = event->pos();
        mc.imgCoordinates = coordRealimage;
        emit imageClicked(mc);
    }
}

void SimpleImageView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(0,0,this->width(),this->height());
    if(!m_im.isNull())
    {

        QImage sim = m_im.scaled(this->size(),Qt::KeepAspectRatio,Qt::FastTransformation);
        int dw = (this->width()-sim.width())/2;
        int dh = (this->height()-sim.height())/2;
        _resized = sim.size();

      //  painter.setBrush(QBrush(m_im));
      //  painter.drawRect(0,0,this->width(),this->height());

        painter.drawImage(QPoint(dw,dh),sim);
    }
    QWidget::paintEvent(event);
}
