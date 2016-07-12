#include "zoomimageview.h"
#include <visual/forms/zoomwidget.h>
#include <types/rgb.h>
#include <types/controls.h>

using namespace visual;
using namespace types;
using namespace visual::ui;

ZoomImageView::ZoomImageView(QObject *parent) :
    ImageVisualNode(parent)
{
    //Create your widget
    ZoomWidget* zw = new ZoomWidget(_invisibleParent);
    connect(this,SIGNAL(sendImage(QImage*)),zw,SLOT(setImage(QImage*)));
    connect(zw,SIGNAL(saveScreen()),this,SLOT(saveImage()));
    connect(this,SIGNAL(sendZoomCenter(QPointF)),zw,SLOT(setZoomCenter(QPointF)));
    _widget = zw;
}
ZoomImageView::~ZoomImageView()
{
    //TODO: Destruction code
    //Do not delete _widget it will be destroyed by VisualNode
}

//This function is called if this node is enable
void ZoomImageView::receiveProcess(esp data)
{
    qDebug() << "Recived smth";
    QSharedPointer<Rgb> var = data.getReadOnly<Rgb>();
    if(!var.isNull())
    {
        emit sendImage(toQImage(var));
    }

    QSharedPointer<QPointF> point = data.getReadOnly<QPointF>();
    if(!point.isNull())
    {
        emit sendZoomCenter(QPointF(point->x(),point->y()));
    }

    QSharedPointer<MouseControl> mouse = data.getReadOnly<MouseControl>();
    if(!mouse.isNull())
    {
        qDebug() << "Recived MouseControl" << QPointF(mouse->imgCoordinates);
        emit sendZoomCenter(QPointF(mouse->imgCoordinates));
    }

    //TODO: show data code inside your _widget
}


