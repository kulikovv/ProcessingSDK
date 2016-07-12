#include "layoutnode.h"
#include <visual/forms/layoutwidget.h>
#include <types/rgb.h>
#include <QDebug>
#include <QGLWidget>

using namespace visual;
using namespace types;
using namespace visual::ui;

LayoutNode::LayoutNode(QObject *parent) :
    ZoomImageView(parent)
{
    //TODO: Initiation source
    //Create your widget
    LayoutWidget* lay = new LayoutWidget(new QGLWidget(_invisibleParent));
    _widget = lay;
	_taborder = 1;
    connect(lay,SIGNAL(saveScreen()),this,SLOT(saveImage()));

    connect(lay,SIGNAL(realParamsChanged(float,float)),this,SLOT(reciveMpxy(float,float)));

    _mpx=1.0;
    _mpy=1.0;
}

QIcon LayoutNode::getIcon()
{
    return QIcon(":/sdk/visual/LayoutNode");
}

LayoutNode::~LayoutNode()
{
    //TODO: Destruction code
    //Do not delete _widget it will be destroyed by VisualNode
}
QString LayoutNode::layout()
{
    LayoutWidget* zoomw = qobject_cast<LayoutWidget*>(_widget);
    if(0!=zoomw)
    {
        return zoomw->toString();
    }
    return "";
}

double LayoutNode::getMpx()
{
    return _mpx;
}

double LayoutNode::getMpy()
{
    return _mpy;
}
void LayoutNode::setMpx(double value)
{
    _mpx = value;
}

void LayoutNode::setMpy(double value)
{
    _mpy = value;
}
void LayoutNode::reciveMpxy(float x,float y)
{
    if(0!=x&&0!=y)
    {
        _mpx = x;
        _mpy = y;
    }
}

QList<double> LayoutNode::getCoordinates(QString name)
{
    QList<double> data;
    data.push_back(0);
    data.push_back(0);
    LayoutWidget* zoomw = qobject_cast<LayoutWidget*>(_widget);
    if(0!=zoomw)
    {
        QPointF pos = zoomw->getPosOfLayer(name);
        data[0]=pos.x();
        data[1]=pos.y();
    }
    return data;
}


double LayoutNode::isInside(QString name,double x,double y)
{
    LayoutWidget* zoomw = qobject_cast<LayoutWidget*>(_widget);
    if(0!=zoomw)
    {
        return zoomw->isInside(name,x,y);
    }
    return 0;
}

void LayoutNode::checkAssignForm(esp data)
{
    if(data=="STRUCT")
    {
        QSharedPointer<QString> string = data.getReadOnly<QString>();
        if(!string.isNull())
        {
            setLayout(*string);
        }
    }

}



void LayoutNode::setLayout(QString data)
{
    LayoutWidget* zoomw = qobject_cast<LayoutWidget*>(_widget);
    if(0!=zoomw)
    {
        zoomw->fromString(data);
    }
}

//This function is called if this node is enable
void LayoutNode::receiveProcess(esp data)
{
    QSharedPointer<Rgb> var = data.getReadOnly<Rgb>();
    if(!var.isNull())
    {
        LayoutWidget* zoomw = qobject_cast<LayoutWidget*>(_widget);
        if(0!=zoomw)
        {
            zoomw->setImage(toQImage(var));
            if(zoomw->hasChanges())
            {
                SEND_TAG(Rgb, new Rgb(zoomw->renderScene("")),"MASK");
                SEND_TAG(QString,new QString(layout()),"STRUCT")
            }
        }

    }else{
        checkAssignForm(data);
    }


}


