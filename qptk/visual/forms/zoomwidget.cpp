#include "zoomwidget.h"
#include <QAction>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QImage>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QSignalMapper>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

using namespace visual::ui;
QAction* ZoomWidget::createAction(QString text,QIcon icon)
{
    QAction *action = new QAction(icon,text,this);
    addAction(action);
    return action;
}

QAction* ZoomWidget::createAction(QString text,QSignalMapper* mapper,int value,QIcon icon)
{
    QAction *action = ZoomWidget::createAction(text,icon);
    mapper->setMapping(action,value);
    return action;
}

ZoomWidget::ZoomWidget(QWidget *parent) :
    QWidget(parent)
{
    _rendersurface = 0;
    _zoomstep = 0.25;

    connect(createAction(tr("Save Image"),QIcon(":/sdk/visual/forms/SaveImage")),SIGNAL(triggered()),this,SIGNAL(saveScreen()));
    connect(createAction(tr("Zoom In"),QIcon(":/sdk/visual/forms/ZoomIn")),SIGNAL(triggered()),this,SLOT(zoomIn()));
    connect(createAction(tr("Zoom Out"),QIcon(":/sdk/visual/forms/ZoomOut")),SIGNAL(triggered()),this,SLOT(zoomOut()));
    connect(createAction(tr("Original"),QIcon(":/sdk/visual/forms/Original")),SIGNAL(triggered()),this,SLOT(toOriginal()));
    connect(createAction(tr("Fit"),QIcon(":/sdk/visual/forms/Fit")),SIGNAL(triggered()),this,SLOT(fit()));

    _scene = new QGraphicsScene(this);
    _view = new QGraphicsView(_scene,this);
    QHBoxLayout* lay =new QHBoxLayout(this);
    lay->setMargin(0);
    lay->addWidget(_view);
    setContextMenuPolicy(Qt::ActionsContextMenu);
}
void ZoomWidget::zoomIn()
{
   _view->scale(1+_zoomstep,1+_zoomstep);
}

void ZoomWidget::zoomOut()
{
   _view->scale(1-_zoomstep,1-_zoomstep);
}

void ZoomWidget::toOriginal()
{
    _view->setTransform(QTransform(1,0,0,0,1,0,0,0,1));
}

void ZoomWidget::fit()
{
    _view->setTransform(QTransform(1,0,0,0,1,0,0,0,1));

    float dx = (float)this->width()/(float)_scene->width();
    float dy = (float)this->height()/(float)_scene->height();
    float minch = dx<dy?dx:dy;

    _view->scale(minch,minch);
}
void ZoomWidget::setSceneRect ( qreal x, qreal y, qreal w, qreal h )
{
    qreal zval=-15;
    if(_rendersurface)
    {
        zval = _rendersurface->zValue();
        _scene->removeItem(_rendersurface);
        delete _rendersurface;
        _rendersurface = NULL;
    }
    _rendersurface = new QGraphicsRectItem(x,y,w,h);
    _rendersurface->setZValue(zval);
    _rendersurface->setParentItem(NULL);

    _scene->addItem(_rendersurface);
    _scene->setSceneRect(x,y,w,h);
   // this->setMinimumSize(w,h);
}
void ZoomWidget::setZoomCenter(QPointF point)
{
	double w = _rendersurface->rect().width();
	double h = _rendersurface->rect().height();
    QPointF realPt(point.x()*w,point.y()*h);
	//QMessageBox::warning(0,"Lol",QString("%1 - %2 -> %3 - %4").arg(point.x()).arg(point.y()).arg(realPt.x()).arg(realPt.y()));
    //qDebug() << "Scene" << realPt;
    _view->centerOn(realPt);
}

void ZoomWidget::setImage(QImage* image)
{
    if(!_rendersurface)
    {
        setSceneRect(0,0,image->width(),image->height());
    }
    if(image->width()!=_rendersurface->boundingRect().width()||image->height()!=_rendersurface->boundingRect().height())
    {
        setSceneRect(0,0,image->width(),image->height());
    }
    _rendersurface->setBrush(QBrush(*image));

}
void ZoomWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Left:
        _view->scroll(-1, 0);
        break;
    case Qt::Key_Right:
        _view->scroll(+1, 0);
        break;
    case Qt::Key_Down:
        _view->scroll(0, -1);
        break;
    case Qt::Key_Up:
        _view->scroll(0, +1);
        break;
    default:
        QWidget::keyPressEvent(event);
    }
    update();
}

void ZoomWidget::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    //int numTicks = numDegrees / 15;

  /*  if(numDegrees>0)
    {
        zoomIn();
    }else{
        zoomOut();
    }*/

   /* if (event->orientation() == Qt::Horizontal) {
        _scene->scroll(numTicks, 0);
    } else {
        _scene->scroll(0, numTicks);
    }*/
   // update();
}
